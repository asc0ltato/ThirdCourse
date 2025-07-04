import logging
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager

# ------------------------- НАСТРОЙКА ЛОГИРОВАНИЯ -------------------------
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# ------------------------- ЗАПУСК БРАУЗЕРА -------------------------
service = Service(ChromeDriverManager().install())
options = Options()
options.add_argument("--start-maximized")
driver = webdriver.Chrome(service=service, options=options)
driver.implicitly_wait(5)  # Неявное ожидание 5 секунд
wait = WebDriverWait(driver, 10)  # Явное ожидание до 10 секунд

# ------------------------- ОТКРЫТИЕ САЙТА -------------------------
driver.get("https://www.saucedemo.com/")

# ------------------------- ПОИСК ЭЛЕМЕНТА ПО NAME -------------------------
# поле логина на странице авторизации
try:
    # Находим по атрибуту name="user-name"
    element_by_name = wait.until(EC.presence_of_element_located((By.NAME, "user-name")))
    assert element_by_name.get_attribute("placeholder") == "Username", "Поле логина не найдено"
    logger.info("Элемент по NAME найден: %s", element_by_name.get_attribute("placeholder"))
except:
    logger.error("Элемент по NAME не найден")

# ------------------------- АВТОРИЗАЦИЯ НА САЙТЕ -------------------------
# для доступа к элементам, которые появляются после входа
try:
    # Находим по уникальному атрибуту id
    username_field = wait.until(EC.presence_of_element_located((By.ID, "user-name")))
    password_field = driver.find_element(By.ID, "password")
    login_button = driver.find_element(By.ID, "login-button")
    # Ввод логина и пароля
    username_field.send_keys("standard_user")
    password_field.send_keys("secret_sauce")
    login_button.click()
    # Ждем появления элемента, который появляется только после входа
    wait.until(EC.presence_of_element_located((By.CLASS_NAME, "inventory_list")))
    logger.info("Авторизация выполнена успешно")
except:
    logger.error("Не удалось выполнить авторизацию")

# 1. Поиск товара по ID (кнопка "Add to cart" для Sauce Labs Backpack)
try:
    element_by_id = wait.until(EC.presence_of_element_located((By.ID, "add-to-cart-sauce-labs-backpack")))
    assert element_by_id.text == "Add to cart", "Текст кнопки не соответствует ожидаемому"
    logger.info("Элемент по ID найден: %s", element_by_id.text)
except:
    logger.error("Элемент по ID не найден")

# 2. Поиск товара по имени класса (название товара)
try:
    element_by_class = wait.until(EC.presence_of_element_located((By.CLASS_NAME, "inventory_item_name")))
    assert element_by_class.text == "Sauce Labs Backpack", "Название товара не соответствует ожидаемому"
    logger.info("Элемент по имени класса найден: %s", element_by_class.text)
except:
    logger.error("Элемент по имени класса не найден")

# 3. Поиск цены товара по сложному CSS-селектору (пример 1)
try:
    element_by_css = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "div.inventory_item:nth-child(1) > div.inventory_item_description > div.pricebar > div.inventory_item_price")))
    assert element_by_css.text == "$29.99", "Цена товара не соответствует ожидаемой (CSS 1)"
    logger.info("Элемент по сложному CSS-селектору 1 найден: %s", element_by_css.text)
except:
    logger.error("Элемент по сложному CSS-селектору 1 не найден")

# 4. Поиск описания товара по сложному CSS-селектору (пример 2)
try:
    element_by_css2 = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "div.inventory_item:nth-child(1) div.inventory_item_description div.inventory_item_desc")))
    expected_desc = "carry.allTheThings() with the sleek, streamlined Sly Pack that melds uncompromising style with unequaled laptop and tablet protection."
    assert element_by_css2.text == expected_desc, "Описание товара не соответствует ожидаемому (CSS 2)"
    logger.info("Элемент по сложному CSS-селектору 2 найден: %s", element_by_css2.text)
except:
    logger.error("Элемент по сложному CSS-селектору 2 не найден")

# 5. Поиск описания товара по сложному XPath (пример 1)
try:
    element_by_xpath = wait.until(EC.presence_of_element_located((By.XPATH, "//div[@class='inventory_item_description']//div[@class='inventory_item_desc']")))
    expected_desc = "carry.allTheThings() with the sleek, streamlined Sly Pack that melds uncompromising style with unequaled laptop and tablet protection."
    assert element_by_xpath.text == expected_desc, "Описание товара не соответствует ожидаемому (XPath 1)"
    logger.info("Элемент по сложному XPath 1 найден: %s", element_by_xpath.text)
except:
    logger.error("Элемент по сложному XPath 1 не найден")

# 6. Поиск цены товара по сложному XPath (пример 2)
try:
    element_by_xpath2 = wait.until(EC.presence_of_element_located((By.XPATH, "//div[@class='inventory_item'][1]//div[@class='pricebar']/div[@class='inventory_item_price']")))
    assert element_by_xpath2.text == "$29.99", "Цена товара не соответствует ожидаемой (XPath 2)"
    logger.info("Элемент по сложному XPath 2 найден: %s", element_by_xpath2.text)
except:
    logger.error("Элемент по сложному XPath 2 не найден")

# 7. Поиск всех товаров по классу (несколько элементов)
try:
    items = wait.until(EC.presence_of_all_elements_located((By.CLASS_NAME, "inventory_item")))
    assert len(items) == 6, f"Ожидалось 6 товаров, найдено {len(items)}"
    logger.info("Найдено %d товаров на странице:", len(items))
    for item in items:
        name = item.find_element(By.CLASS_NAME, "inventory_item_name").text
        price = item.find_element(By.CLASS_NAME, "inventory_item_price").text
        logger.info("Товар: %s, Цена: %s", name, price)
except:
    logger.error("Товары не найдены")

# 8. Поиск по частичному тексту ссылки
try:
    element_by_partial_link = wait.until(EC.presence_of_element_located((By.PARTIAL_LINK_TEXT, "Sauce Labs")))
    assert "Sauce Labs" in element_by_partial_link.text, "Текст ссылки не содержит 'Sauce Labs'"
    logger.info("Элемент по частичному тексту ссылки найден: %s", element_by_partial_link.text)
except:
    logger.error("Элемент по частичному тексту ссылки 'Sauce Labs' не найден")

# 9. Поиск нескольких элементов (социальные сети)
try:
    elements = wait.until(EC.presence_of_all_elements_located((By.CLASS_NAME, "social_twitter")))
    assert len(elements) == 1, f"Ожидался 1 элемент с классом 'social_twitter', найдено {len(elements)}"
    logger.info("Найдено %d элементов с классом 'social_twitter':", len(elements))
    for elem in elements:
        logger.info("Элемент: %s", elem.text)
except:
    logger.error("Элементы с классом 'social_twitter' не найдены")

# Закрытие браузера
driver.quit()