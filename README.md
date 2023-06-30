# Deformador do LITel

Esse repositório guarda o que foi desenvolvido de parte eletrônica / de código
do deformador do LITel.

# Informações

O deformador é um equipamento construído para aplicar uma deformação controlada
a uma placa de teste. A placa é afixada em uma extremidade, enquanto que a
outra é conectada a um cabo de aço que pode fazer uma força para baixo,
deformando o material com uma flecha. Um dinamômetro entre a placa e o cabo de
aço mede a força constantemente, para que o sistema possa ser usado para
calibração de sensores. O cabo de aço é puxado por um sistema com um motor de
passo e um fuso, controlado por um esp8266 através de duas interfaces
possíveis.

Embora o sistema tenha a possibilidade de visualização do movimento do fuso do
motor, é recomendado que se use apenas a força lida no dinamômetro para
calibração de sensores. Isso se dá porque a distância percorrida no fuso não
é igual à distância da flecha na placa, por conta dos componentes que estão no
meio do caminho.

# Interface Física

A interface mais simples de se usar é a física, composta de 7 botões. Com o
lado dos botões para cima, e o conector à esquerda, a função dos botões são,
seguindo da esquerda para a direita:

- Botão 1: Recua 5cm (alivia a carga);
- Botão 2: Recua 5mm (alivia a carga);
- Botão 3: Recua 1mm (alivia a carga);
- Botão 4: Interrompe o movimento;
- Botão 5: Avança 1mm (aumenta a carga);
- Botão 6: Avança 5mm (aumenta a carga);
- Botão 7: Avança 5cm (aumenta a carga);

Caso os botões sejam segurados, os comandos serão continuamente repetidos,
sendo que os valores são atualizados a partir do atual. Dessa forma, o sistema
irá iniciar o movimento ao pressionar o botão, e irá se mover pelo valor do
botão após ser soltado.

> Por exemplo, se o botão 3 for segurado por 2 segundos, e então solto, o
> sistema irá recuar continuamente por 2 segundos, e após isso ainda irá recuar
> mais 1mm.

Os botões mais extremos movem a flecha por valores consideravelmente grandes, e
foram feitos com o objetivo de colocar um alvo distante e de que os comandos
sejam interrompidos manualmente.

Com essa interface, não há como saber a distância percorrida no fuso do motor.

# Interface Web

Para o uso da interface web, é necessário o uso de um dispositivo que esteja
conectado à mesma rede local do esp8266 e seja capaz de renderizar páginas web
(HTML, CSS e JS). O esp possui a função de ponto de acesso, permitindo a
que o dispositivo se conecte diretamente a ele, caso possua conectividade WiFi.

## Ponto de Acesso (Access Point)

As credenciais de acesso do modo de ponto de acesso são as seguintes:

> SSID: deformador-litel
> Senha: 787Cu7kg

Nesse modo, o programa pode ser acessado através do ip:

> ip: 192.168.4.1

## Modo Estação (Station)

O esp também pode se conectar a uma rede externa. Isso aumenta a confiabilidade
do sistema, porque o ponto de acesso do esp não é muito estável. Para que ele
se conecte, é necessário que primeiro o dispositivo acesse o programa pelo
ponto de acesso, e então configure as credenciais da rede externa pela página
de conexão. Na página de conexão é possível ver o status (se conseguiu
conectar) e o endereço de ip.

Com o dispositivo conectado à mesma rede externa, o endereço ip do esp deverá
ser utilizado para acessar a aplicação.

> O esp tentará se conectar à última rede configurada sempre que for ligado.

## Páginas

### Página Principal

A página principal serve apenas como entrada à interface. Ela permite acesso às
outras páginas do sistema, e também tem o link do repositório no GitHub.

> Todas as páginas possuem os mesmos links presentes nessa.

### Conexão

Essa página mostra as informações de conexão (SSID, senha, ip), e permite mudar
as credenciais da rede a ser acessada pelo modo estação. Também indica se o
modo de estação está conectado devidamente.

### Controle

Essa página é a que permite a interface principal do sistema. Através daqui é
possível visualizar a posição atual do motor, o deslocamento correspondente no
fuso, e também enviar novos comandos para o sistema.

Os componentes importantes da página são:

- Comandos: É possível enviar dois comandos para o sistema:
  - Deslocar: Envia uma nova posição alvo para o deslocamento. Esse comando não
              é cumulativo, o valor enviado é o alvo, não um valor a mover;
  - Encontrar zero: Envia o comando de reset de zero. O sistema entrará na
                    sequência de detecção, não podendo receber outros comandos
                    e recuando (aliviando a carga) até que encontrar o sensor
                    de fim de curso;
- Informações: Algumas informações são dispostas no centro da página:
  - Progresso: Uma barra cresce da esquerda para a direita, indicando o
               progresso do último comando;
  - Estado: O estado do sistema, que pode ser "movendo", "parado" ou
            "procurando zero"
  - Posição: A posição e correspondente deslocamento atuais no fuso do motor.

> !! O DESLOCAMENTO NÃO DEVE SER USADO PARA CALIBRAÇÃO !! É possível calcular a
> deformação da placa a partir da flecha, mas não é trivial calcular a flecha
> a partir do deslocamento do fuso. Os deslocamentos não serão iguais, porque
> o cabo de aço, o dinamômetro e outros componentes no meio do caminho se
> deformam
