//Кнопка scroll
const scroll_up = document.querySelector(".scroll");
scroll_up.addEventListener("click", fun1);
window.onscroll = () => {
  if (document.documentElement.scrollTop > 200)
    scroll_up.style.display = "block";
  else {
    scroll_up.style.display = "none";
  }
};

//Функция, чтобы scroll поднимал нас наверх
function fun1() {
  window.scrollTo({
    top: 0,
    behavior: "smooth",
  });
}