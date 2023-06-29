document.addEventListener("DOMContentLoaded", function(event) {
    document.querySelector("#sta_config_form").onsubmit = submit_sta_config;
    get_ap_info();
    setTimeout(() => get_sta_info(true), 500)   // Esperando pra n sobrecarregar o servidor

    setTimeout(() => setInterval(get_sta_info, 10000), 10000)
});


function get_ap_info(){
    // Busca as informacoes do modo ap do esp

    const request = new Request("api/ap_info", {
        method: "get"
    });

    fetch(request)
    .then((response) => response.json())
    .then((result) => {
        document.querySelector("#ap_ssid").innerHTML = result["ssid"];
        document.querySelector("#ap_password").innerHTML = result["password"];
        document.querySelector("#ap_ip").innerHTML = result["ip"];
    });
}


function get_sta_info(get_ssid=false){
    // Busca as informacoes do modo sta do esp

    const request = new Request("api/sta_info", {
        method: "get"
    });

    fetch(request)
    .then((response) => response.json())
    .then((result) => {
        if(get_ssid){
            document.querySelector("#sta_ssid").value = result["ssid"];
            document.querySelector("#sta_password").value = result["password"];
        }

        document.querySelector("#sta_submit").innerHTML = "Enviar";
        const ip_value = result["ip"].length > 1 ? result["ip"] : "<span class='error'>Desconectado</span>";
        document.querySelector("#sta_ip").innerHTML = ip_value;
    }).catch(reason =>{
        console.log("Falhou na busca das informações de sta.");
        console.log(reason);

        const ip_value = "<span class='error'>Desconectado</span>";
        document.querySelector("#sta_ip").innerHTML = ip_value;

        document.querySelector("#sta_submit").innerHTML = "<span class='error'>Servidor Inalcançável</span>";
    });
}


function submit_sta_config(){
    // Envia as novas configuracoes para conexão do modo sta do esp

    const sta_ssid = document.querySelector("#sta_ssid").value;
    const sta_password = document.querySelector("#sta_password").value;

    const request = new Request("api/sta_info", {
        method: "POST",
        body: JSON.stringify({
            ssid: sta_ssid,
            password: sta_password
        })
    });

    fetch(request)
    .then((response) => response.json())
    .then((result) =>{
        console.log(result);
    });

    document.querySelector("#sta_ip").innerHTML = "<span class='error'>Desconectado</span>";

    setTimeout(()=>get_sta_info(true), 500)
    return false;
}
