import logging
import unittest
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

class TestSauceDemo(unittest.TestCase): # Наследуем
    @classmethod
    def setUpClass(cls):
        service = Service(ChromeDriverManager().install())
        options = Options()
        options.add_argument("--start-maximized")
        cls.driver = webdriver.Chrome(service=service, options=options)
        cls.driver.implicitly_wait(5)  # Неявное ожидание 5 секунд
        cls.wait = WebDriverWait(cls.driver, 20)  # Явное ожидание до 10 секунд

    def setUp(self):
        # Очистка состояния перед каждым тестом
        self.driver.get("https://www.saucedemo.com/")

    # Функция для перехода на страницу товаров
    def go_to_inventory_page(self):
        self.driver.get("https://www.saucedemo.com/inventory.html")
        try:
            self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "inventory_list")))
            logger.info("Перешли на страницу товаров")
        except:
            logger.error("Не удалось перейти на страницу товаров (возможно, пользователь заблокирован)")

    # Функция авторизации
    def login(self, username, password):
        # Проверяем, авторизованы ли уже (есть ли элемент logout_link)
        if self.driver.find_elements(By.ID, "logout_sidebar_link"):
            logger.info("Уже авторизованы, выполняем выход")
            menu = self.driver.find_element(By.ID, "react-burger-menu-btn")
            menu.click()
            logout_link = self.wait.until(EC.element_to_be_clickable((By.ID, "logout_sidebar_link")))
            logout_link.click()
            # Ждем, пока страница логина полностью загрузится
            self.wait.until(EC.presence_of_element_located((By.ID, "user-name")))
            self.wait.until(EC.presence_of_element_located((By.ID, "password")))
            logger.info("Выход выполнен")

        logger.info("Авторизация пользователя %s", username)
        self.driver.get("https://www.saucedemo.com/")
        # Ждем, пока поля логина и пароля будут доступны
        username_field = self.wait.until(EC.presence_of_element_located((By.ID, "user-name")))
        password_field = self.wait.until(EC.presence_of_element_located((By.ID, "password")))
        login_button = self.wait.until(EC.element_to_be_clickable((By.ID, "login-button")))
        username_field.send_keys(username)
        password_field.send_keys(password)
        login_button.click()
        # Проверяем, успешна ли авторизация
        try:
            self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "inventory_list")))
            logger.info("Авторизация пользователя %s выполнена успешно", username)
            return True
        except:
            # Проверяем наличие элемента ошибки
            try:
                error_message = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "error-message-container"))).text
            except:
                error_message = "Не удалось найти сообщение об ошибке"
            logger.error("Ошибка авторизации для %s: %s", username, error_message)
            return False

    def test_login_users(self):
        logger.info("Starting test_login_users")
        users = [
            ("standard_user", "secret_sauce"),
            ("locked_out_user", "secret_sauce"),
            ("problem_user", "secret_sauce")
        ]
        for username, password in users:
            success = self.login(username, password)
            if username == "standard_user" or username == "problem_user":
                self.assertTrue(success, f"Авторизация для {username} должна быть успешной")
            else:  # locked_out_user
                self.assertFalse(success, f"Авторизация для {username} должна завершиться ошибкой")
        logger.info("test_login_users passed")

    def test_inventory_title(self):
        logger.info("Starting test_inventory_title")
        self.login("standard_user", "secret_sauce")
        self.go_to_inventory_page()
        title = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "title")))
        self.assertEqual(title.text, "Products", "Заголовок страницы товаров неверный")
        logger.info("test_inventory_title passed")

    def test_cart_button_presence(self):
        logger.info("Starting test_cart_button_presence")
        self.login("standard_user", "secret_sauce")
        self.go_to_inventory_page()
        cart_button = self.wait.until(EC.presence_of_element_located((By.ID, "shopping_cart_container")))
        self.assertTrue(cart_button.is_displayed(), "Кнопка корзины не найдена")
        logger.info("test_cart_button_presence passed")

    def test_add_to_cart(self):
        logger.info("Starting test_add_to_cart")
        self.login("standard_user", "secret_sauce")
        self.go_to_inventory_page()
        add_button = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//button[contains(text(), 'Add to cart')]")))
        add_button.click()
        cart_link = self.wait.until(EC.element_to_be_clickable((By.CLASS_NAME, "shopping_cart_link")))
        cart_link.click()
        cart_item = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "inventory_item_name")))
        self.assertTrue(cart_item.is_displayed(), "Товар не добавлен в корзину")
        logger.info("test_add_to_cart passed")

    def test_sort_dropdown(self):
        logger.info("Starting test_sort_dropdown")
        self.login("standard_user", "secret_sauce")
        self.go_to_inventory_page()

        price_elements_before = self.wait.until(
            EC.presence_of_all_elements_located((By.CLASS_NAME, "inventory_item_price")))
        prices_before = [float(p.text.replace("$", "")) for p in price_elements_before]

        expected_prices = sorted(prices_before)
        logger.info("Цены товаров до сортировки: %s", prices_before)
        logger.info("Ожидаемые цены после сортировки (отсортированные): %s", expected_prices)

        dropdown = self.wait.until(EC.element_to_be_clickable((By.CLASS_NAME, "product_sort_container")))
        dropdown.click()
        option = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//option[@value='lohi']")))
        option.click()

        price_elements_after = self.wait.until(
            EC.presence_of_all_elements_located((By.CLASS_NAME, "inventory_item_price")))
        prices_after = [float(p.text.replace("$", "")) for p in price_elements_after]
        logger.info("Цены товаров после сортировки: %s", prices_after)

        self.assertEqual(prices_after, expected_prices, f"Ожидались цены {expected_prices}, но получены {prices_after}")

    @classmethod
    def tearDownClass(cls):
        # Закрытие браузера
        cls.driver.quit()

if __name__ == "__main__":
    unittest.main()