var nbSecondes = 20;
var demarrer = false;


function getFromESP_ListeWood(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var reponse = this.responseText;
        var reponse = reponse.slice(0, -1);
        var lesBois = reponse.split(";");
        var x = document.getElementById("liste_bois");

        for (let i = 0; i < lesBois.length; i++) {
          var option = document.createElement("option");
          option.text = lesBois[i];
          x.add(option);
          }
    }
    };
    xhttp.open("GET", "getListeWood", true);
    xhttp.send();
}

function afficherBois(){
    nomBois = document.getElementById("liste_bois").value;
    var params = String("nomBois") + String("=") + String(nomBois);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var reponse = this.responseText;
        var reponse = reponse.slice(0, -1);
        var lesBois = reponse.split(";");
        document.getElementById("carac_bois").innerHTML = lesBois[0];
        document.getElementById("bois").innerHTML = lesBois[0];
        document.getElementById("carac_type").innerHTML = lesBois[1];
        document.getElementById("carac_origine").innerHTML = lesBois[2];
        document.getElementById("carac_tempsSechage").innerHTML = lesBois[3];
        document.getElementById("tempsSechage").innerHTML = lesBois[3];
        document.getElementById("carac_temperatureMin").innerHTML = lesBois[4];
        document.getElementById("temperatureMin").innerHTML = lesBois[4];

    }
    };
    xhttp.open("POST", "afficherBois", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
}



setInterval(function getTemperature(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.status == 200) {
    var myResponse = this.responseText.split(";");
    document.getElementById("temp_celcius").innerHTML = myResponse[0];
    document.getElementById("secondes").innerHTML = myResponse[1];
    }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();

   }, 1000);

function doAction(actionToDo) {
    var params = String("actionToDo") + String("=") + String(actionToDo);
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "ActionToDo", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
    demarrer = true;
}
