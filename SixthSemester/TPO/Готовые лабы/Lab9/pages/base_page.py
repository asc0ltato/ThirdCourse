from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException


class BasePage:
    def __init__(self, driver, url=None, timeout=10):
        self.driver = driver
        self.timeout = timeout
        self.wait = WebDriverWait(self.driver, self.timeout)
        if url:
            self.driver.get(url) # открываем этот URL в браузере

    def find_element(self, locator, condition=EC.presence_of_element_located, timeout=None):
        # Ищем элемент с ожиданием, по умолчанию ждем присутствия
        try:
            wait = self.wait if timeout is None else WebDriverWait(self.driver, timeout)
            return wait.until(condition(locator))
        except TimeoutException:
            raise TimeoutException(f"Элемент с локатором {locator} не найден за {timeout or self.timeout} секунд.")

    def find_elements(self, locator, condition=EC.presence_of_all_elements_located, timeout=None):
        try:
            wait = self.wait if timeout is None else WebDriverWait(self.driver, timeout)
            return wait.until(condition(locator))
        except TimeoutException:
            raise TimeoutException(f"Элементы с локатором {locator} не найдены за {timeout or self.timeout} секунд.")

    def click_element(self, locator):
        # Ждем, пока элемент станет кликабельным, и кликаем
        element = self.find_element(locator, EC.element_to_be_clickable)
        element.click()

    def send_keys_to_element(self, locator, keys):
        # Ждем видимости и вводим текст
        element = self.find_element(locator, EC.visibility_of_element_located)
        element.send_keys(keys)

    def get_element_text(self, locator):
        # Получаем текст видимого элемента
        element = self.find_element(locator, EC.visibility_of_element_located)
        return element.text

    def get_current_url(self):
        # Текущий URL в браузере
        return self.driver.current_url

    def wait_for_url_to_be(self, expected_url):
        # Ждем пока URL станет строго равен expected_url
        try:
            self.wait.until(EC.url_to_be(expected_url))
        except TimeoutException:
            raise TimeoutException(f"URL не изменился на {expected_url} за {self.timeout} секунд. Текущий URL: {self.driver.current_url}")

    def wait_for_staleness(self, element):
        # Ждем, пока элемент не исчезнет из DOM
        try:
            self.wait.until(EC.staleness_of(element))
        except TimeoutException:
            print(f"Элемент {element} не стал устаревшим за {self.timeout} секунд.")

    def wait_for_url_contains(self, url_part, timeout=None):
        # Ждем, пока URL содержит часть строки url_part
        try:
            effective_wait = self.wait if timeout is None else WebDriverWait(self.driver, timeout)
            effective_wait.until(EC.url_contains(url_part))
        except TimeoutException:
            effective_timeout = timeout or self.timeout
            raise TimeoutException(f"URL не стал содержать '{url_part}' за {effective_timeout} секунд. Текущий URL: {self.driver.current_url}")

    def take_screenshot(self, filename="screenshot.png"):
        # Создаем папку screenshots если нет и сохраняем скриншот
        import os
        screenshot_dir = "screenshots"
        if not os.path.exists(screenshot_dir):
            os.makedirs(screenshot_dir)
        filepath = os.path.join(screenshot_dir, filename)
        self.driver.save_screenshot(filepath)
        print(f"Скриншот сохранен: {filepath}")

    def execute_script(self, script, *args):
        # Выполнение JS кода в браузере
        return self.driver.execute_script(script, *args)

    def scroll_into_view(self, locator):
        # Скроллим страницу до элемента
        element = self.find_element(locator)
        self.execute_script("arguments[0].scrollIntoView(true);", element)

    def get_cookies(self):
        # Получить куки из браузера
        return self.driver.get_cookies()

    def add_cookie(self, cookie_dict):
        # Добавить куку в браузер
        self.driver.add_cookie(cookie_dict)

    def delete_all_cookies(self):
        # Удалить все куки
        self.driver.delete_all_cookies()