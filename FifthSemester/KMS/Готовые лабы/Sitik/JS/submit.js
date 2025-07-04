const sendEmailButton = document.getElementById("submit");

sendEmailButton.addEventListener("click", function(event) {
event.preventDefault();

const recipient = "superpando4ka@gmail.com"; 
const subject = "Вопрос по лабораторной установке";
const body = "Текст письма"; 

const mailtoLink = `mailto:${recipient}?subject=${encodeURIComponent(subject)}&body=${encodeURIComponent(body)}`;

window.location.href = mailtoLink;
});