var nbSecondes = 20;
var demarrer = false;

function getFromESP_getNom () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    document.getElementById("temp_text").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "getNomEsp", true);
    xhttp.send();
   }


function Liste_Bois_Select(item, index){
    var x = document.getElementById("liste_bois");
var option = document.createElement("option");
option.text = item;
x.add(option);
}



setInterval(function getNomSysteme(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.status == 200) {
    document.getElementById("temp_celcius").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();

    if(demarrer){
        if(nbSecondes == 0){
            document.getElementById("secondes").innerHTML = 0 + "s";
            nbSecondes = 20;
            demarrer = false;
        }else{
            document.getElementById("secondes").innerHTML = nbSecondes + "s";
            nbSecondes--;
        }

    }
   }, 1000);

function doAction(actionToDo) {
    var params = String("actionToDo") + String("=") + String(actionToDo);
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "ActionToDo", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
    demarrer = true;
    }

function countSecondes(){
    for (let i = 20; i > 0; i--) {
        document.getElementById("secondes").innerHTML = i + "s";
    }
}