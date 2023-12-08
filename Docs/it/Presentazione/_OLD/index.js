const sitemap = [
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [1,1,1],
    [1,1,1],
    [0,1,0]
];
    
const simapY = sitemap.length;
const simapX = sitemap[0].length;

const main_body = document.getElementById('content');
    
var old_postion = [1,0];
var position = [0,0];
var new_position = [1,0];

function render ( ) 
{ main_body.setAttribute("style", "transform: translate3d(-" + (100 * new_position[0]) + "vw,-" + (100 * new_position[1]) + "vh,0)"); }
    
function navigate ( ) {
    // validate new poistion
    new_position[0] = old_postion [0] + position [0];
    new_position[1] = old_postion [1] + position [1];
    
    // clamping
    new_position[0] < 0 ? new_position[0] = 0 : (new_position[0] >= simapX ? new_position [0] = simapX -1 : true);
    new_position[1] < 0 ? new_position[1] = 0 : (new_position[1] >= simapY ? new_position [1] = simapY -1 : true);
        
    if 
    ( sitemap [new_position [1]] [new_position[0]] == 0 ) { 
        new_position[0] = old_postion[0];
        new_position[1] = old_postion[1];
    } else {
        old_postion[0] = new_position[0];
        old_postion[1] = new_position[1];
    }
    position = [0,0];
    render ( );
}    

function key_proces ( key ) {
    if (key == "ArrowUp") 
    { position[1] --; }
    if (key == "ArrowDown") 
    { position[1] ++; }
    if (key == "ArrowRight") 
    { position[0] ++; }
    if (key == "ArrowLeft") 
    { position[0] --; }

    // animate autor transition: 
    console.log ( old_postion)
    if ( key == "ArrowDown" ) {
        if ( old_postion[0] == 1 && old_postion[1] == 0 ) { console.log("m a"); document.getElementById("falsa_storia").classList.add("active") }
        if ( old_postion[0] == 1 && old_postion[1] == 2 ) { console.log("r a"); document.getElementById("falsa_storia_2").classList.add("active") }
    }

    if ( key == "ArrowUp" ) {
        document.getElementById("falsa_storia").classList.remove("active")
        document.getElementById("falsa_storia_2").classList.remove("active")
    }

    navigate();
}

document.addEventListener('keydown', function(event) { key_proces(event.key); });
    
render();

var _parlamient = 0;

function parlamient () {
    console.log(_parlamient)
    if (_parlamient == 3 ) {
        document.getElementById('parlamient').classList.add("active");
    }
}

function covoni () {
    console.log ('covoini');
    document.getElementById("covoni").classList.add("active");
    document.getElementById("covoni_panel").classList.add("active");

    key_proces("ArrowRight");
    _parlamient++;
    parlamient();
}

function ninfee () {
    console.log ('nifee');
    document.getElementById("ninfe").classList.add("active");
    document.getElementById("ninfe_panel").classList.add("active");

    key_proces("ArrowDown");
    _parlamient++;
    parlamient();
}

function rouen () {
    console.log ('rouen');
    document.getElementById("rouen").classList.add("active");
    document.getElementById("rouen_panel").classList.add("active");

    key_proces("ArrowLeft");
    _parlamient++;
    parlamient();
}

var covone_corrente = 0;
function prossimo_covone ( ) {
    const covoni = document.getElementsByClassName ("covone");
    const covoni_i = document.querySelectorAll (".covone #immagine")
    const covoni_t = document.querySelectorAll (".covone div h2")
    const covoni_d = document.querySelectorAll (".covone div h4")

    covoni[covone_corrente].classList.add('none');
    covoni_i[covone_corrente].classList.remove('zoom');
    covoni_t[covone_corrente].classList.remove('inactive');
    covoni_d[covone_corrente].classList.remove('inactive');

    if ( covone_corrente < covoni_i.length - 1 ) 
    { covone_corrente++; }
}

var rouen_corrente = 0;
function prossimo_rouen ( ) {
    const roueni = document.getElementsByClassName ("rouen");
    const roueni_i = document.querySelectorAll (".rouen #immagine")
    const roueni_t = document.querySelectorAll (".rouen div h2")
    const roueni_d = document.querySelectorAll (".rouen div h4")
    
    roueni[rouen_corrente].classList.add('none');
    roueni_i[rouen_corrente].classList.remove('zoom');
    roueni_t[rouen_corrente].classList.remove('inactive');
    roueni_d[rouen_corrente].classList.remove('inactive');

    if ( rouen_corrente < roueni_i.length - 1 ) 
    { rouen_corrente++; }
    console.log(roueni_t)
}

var ninfe_corrente = 0;
function prossimo_ninfe ( ) {
    const ninfee = document.getElementsByClassName ("ninfe");
    const ninfee_i = document.querySelectorAll (".ninfe #immagine")
    const ninfee_t = document.querySelectorAll (".ninfe div h2")
    const ninfee_d = document.querySelectorAll (".ninfe div h4")

    ninfee[ninfe_corrente].classList.add('none');
    ninfee_i[ninfe_corrente].classList.remove('zoom');
    ninfee_t[ninfe_corrente].classList.remove('inactive');
    ninfee_d[ninfe_corrente].classList.remove('inactive');

    if ( ninfe_corrente < ninfee_i.length - 1 ) 
    { ninfe_corrente++; }
}

var parlamient_corrente = 0;
var ppp = 0;
function prossimo_parlamient ( ) {
    const parlamiente = document.getElementsByClassName ("parlamient");
    const parlamiente_i = document.querySelectorAll (".parlamient #immagine")
    const parlamiente_t = document.querySelectorAll (".parlamient div h2")
    const parlamiente_d = document.querySelectorAll (".parlamient div h4")

    parlamiente[parlamient_corrente].classList.add('none');
    parlamiente_i[parlamient_corrente].classList.remove('zoom');
    parlamiente_t[parlamient_corrente].classList.remove('inactive');
    parlamiente_d[parlamient_corrente].classList.remove('inactive');

    if ( ppp < parlamiente_i.length - 1 ) 
    { parlamient_corrente++; }

    if ( ppp == parlamiente_i.length )
    { parlamiente[parlamient_corrente+1].classList.add('fade'); }

    if ( ppp == parlamiente_i.length + 1 )
    { end(); }


    ppp++;
}

function grenouillère_renoir () {
    document.getElementById("renoir").classList.add("scat");
    key_proces("ArrowRight");
} 

function grenouillère_monet () {
    console.log("monet");
    document.getElementById("monet").classList.add("scat");
    key_proces("ArrowLeft");
} 

function end () {
    document.getElementById("crediti").classList.add("active");
}
