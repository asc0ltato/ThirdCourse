from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager

# ------------------------- ЗАПУСК БРАУЗЕРА -------------------------
service = Service(ChromeDriverManager().install())
options = Options()
options.add_argument("--start-maximized")
driver = webdriver.Chrome(service=service, options=options)
driver.implicitly_wait(5)  # Неявное ожидание 5 секунд
wait = WebDriverWait(driver, 10)  # Явное ожидание до 10 секунд

# Тест 1: Проверка radio-button на demoqa.com
def test_radio_button():
    print("Starting test_radio_button")
    driver.get("https://demoqa.com/radio-button")
    # Ждем загрузки страницы и видимости элемента
    yes_label = wait.until(EC.element_to_be_clickable((By.XPATH, "//label[@for='yesRadio']")))
    # Прокручиваем страницу до элемента
    driver.execute_script("arguments[0].scrollIntoView(true);", yes_label)
    # Дополнительное ожидание после прокрутки
    wait.until(EC.element_to_be_clickable((By.XPATH, "//label[@for='yesRadio']")))
    yes_label.click()
    result = wait.until(EC.presence_of_element_located((By.CLASS_NAME, "text-success")))
    assert result.text == "Yes", "Radio button 'Yes' не выбран"
    print("test_radio_button passed")

# Тест 2: Проверка check-box на demoqa.com
def test_check_box():
    print("Starting test_check_box")
    driver.get("https://demoqa.com/checkbox")
    # Ждем загрузки страницы и видимости элемента
    home_checkbox = wait.until(EC.element_to_be_clickable((By.XPATH, "//span[text()='Home']/preceding-sibling::span[@class='rct-checkbox']")))
    # Прокручиваем страницу до элемента
    driver.execute_script("arguments[0].scrollIntoView(true);", home_checkbox)
    # Дополнительное ожидание после прокрутки
    wait.until(EC.element_to_be_clickable((By.XPATH, "//span[text()='Home']/preceding-sibling::span[@class='rct-checkbox']")))
    home_checkbox.click()
    result = wait.until(EC.presence_of_element_located((By.ID, "result")))
    assert "home" in result.text.lower(), "Check-box 'Home' не выбран"
    print("test_check_box passed")

# Запуск тестов
test_radio_button()
test_check_box()

# Закрытие браузера
driver.quit()