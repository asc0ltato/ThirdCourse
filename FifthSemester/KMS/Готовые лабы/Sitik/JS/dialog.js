var dialogOn = false;

function dialog_window() {
    document.body.innerHTML += `
        <div id='dialog' class='dialog' style='margin-left:-37px;'>
            <div class='label' onclick='openDialog()'>Нажми, чтобы спросить!</div>
            <div class='headerr'>История:</div>
            <div class='history' id='history'></div>
            <div class='question'>
                <input id='MyDialog' placeholder='Введите вопрос'/> <br>
                <button onclick='ask("MyDialog")'>Спросить</button>
            </div>
        </div>
        <div id='zoom-container' class='zoom-container'>
            <div class='bg' onclick='hide()'></div>
            <img id='zoomed-image' class='zoomed-image' />
        </div>
    `;
  window.ya.speechkit.settings.apikey = "ВСТАВЬТЕСВОЙКЛЮЧ";
  var textline = new ya.speechkit.Textline("MyDialog", {
    onInputFinished: function (text) {
      document.getElementById("MyDialog").value = text;
    },
  });
}

function openDialog() {
  if (dialogOn) {
    $("#dialog").animate({ "margin-left": "-37px" }, 1000, function () {});
    dialogOn = false;
  } else {
    $("#dialog").animate({ "margin-left": "-75vw" }, 1000, function () {});
    dialogOn = true;
    clearInterval(timer);
  }
}

function ask(questionInput) {
  var question = document.getElementById(questionInput).value;
  dialogOn = true;
  var newDiv = document.createElement("div");
  newDiv.className = "question";
  newDiv.innerHTML = question;
  document.getElementById("history").appendChild(newDiv);
  +"</div>";
  newDiv = document.createElement("div");
  newDiv.className = "answer";
  newDiv.innerHTML = getAnswer(question);
  newDiv.innerHTML +=
    "<audio controls='true' autoplay='true' " +
    "src='http://tts.voicetech.yandex.net/" +
    "generate?format=wav&lang=ru-RU&" +
    "key=ВСТАВЬТЕСВОЙКЛЮЧ" +
    "text=" + newDiv.innerText + "'></audio>";
    const images = newDiv.querySelectorAll("img");
    images.forEach(img => {
        img.onclick = function () {
            zoom(this.src);
        };
    });
  document.getElementById("history").appendChild(newDiv);
  if (newDiv.lastChild.tagName == "audio") {
    newDiv.lastChild.play();
  }
  document.getElementById("history").scrollTop = 
  document.getElementById("history").scrollHeight;
  document.getElementById(questionInput).value = "";
}

function zoom(imageSrc) {
  const zoomedImage = document.getElementById("zoomed-image");
  zoomedImage.src = imageSrc;
  document.getElementById("zoom-container").style.display = "block";
}

function hide() {
  document.getElementById("zoom-container").style.display = "none";
}