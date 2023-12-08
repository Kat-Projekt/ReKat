var panel_counter = 0;
var max_panel = 7;
const main_body = document.getElementById("content");

function update_counter ( p ) {
    if ( panel_counter + p < 0 ) { return; }
    if ( panel_counter + p > max_panel ) { return; }
    panel_counter += p;
    main_body.setAttribute("style", "transform: translate3d(-" + (100 * panel_counter) + "vw,0,0");
    console.log (panel_counter)
}

function next () {
    console.log ("nexxt");
    update_counter (1);
}

function previous () {
    console.log ("pre")
    update_counter (-1);
}