document.addEventListener("DOMContentLoaded", function(event) {
    document.querySelector("#operation_info").onsubmit = submit_deformation;

    update_interface();
    setInterval(update_interface, 500);
});

function step_to_deformation(step) {
                                // 2000 um per turn, 200*32 steps per turn
    return step * 2000 / (200 * 32);
}

function deformation_to_step(deformation) {
                                // 200*32 steps per turn, 2000 um per turn
    return Math.round(deformation * 200 * 32 / 2000);
}

function submit_deformation(){
    // Envia a deformação alvo

    const deformation = parseFloat(
        document.querySelector("#deformation_div input").value
    );
    const step = deformation_to_step(deformation);

    if(step === NaN) {
        alert("Digite um número");
        return false;
    }

    const request = new Request("/api/send_target", {
        method: "POST",
        body: JSON.stringify({
            target: step
        })
    });

    fetch(request)
    .then((response) => {
        if(response.status != 200){
            alert("Envio falhou, status" + response.status);
        }
    }).catch((reason) => {
        alert("Envio falhou, não recebeu resposta do servidor");
    })

    // Manda ignorar o efeito padrão do submit
    return false;
}


var last_step_start = 0;
var last_target = 0;

function update_interface() {
    // Adquire algumas informacoes do servidor para atualizar a interface

    const request = new Request("api/controller_info", {
        method: "get"
    });

    fetch(request)
    .then((response) => response.json())
    .then((result) => {
        const state = result["state"];
        if(state === 0){
            document.querySelector("#moving").innerHTML = "Parado";
        } else if (state === 1){
            document.querySelector("#moving").innerHTML = "Movendo";
        } else if (state === 2) {
            document.querySelector("#moving").innerHTML = "Procurando 0";
        }

        const position = result["real_position"];
        const target = result["target_position"];

        if(result["target_position"] != last_target) {
            last_target = result["target_position"];
            last_step_start = result["real_position"];
        }

        const progress = (position - last_step_start) / (target - last_step_start);
        const ratio = 100 * progress;
        document.querySelector("#deformation_bar").style.width = ratio + "%";

        const deformation = step_to_deformation(result["real_position"]).toFixed(2);
        document.querySelector("#deformation").innerHTML = deformation + "μm";

        const step = result["real_position"];
        document.querySelector("#step").innerHTML = step;

    }).catch((reason) => {
       console.log("Não obteve resposta do servidor." + reason);
    })
}

function reset_zero() {
    const request = new Request("/api/reset_zero", {
        method: "POST"
    });

    fetch(request)
    .then((response) => {
        if(response.status != 200){
            alert("Envio falhou, status" + response.status);
        }
    }).catch((reason) => {
        alert("Envio falhou, não recebeu resposta do servidor");
    })

    // Manda ignorar o efeito padrão do submit
    return false;
}

