//http://dicom.nema.org/Dicom/2013/output/chtml/part06/chapter_B.html

(() => {

    let file = "";

    var selector = "body > div.chapter > div.section > div:nth-child(5) > div:nth-child(3) > div:nth-child(8) > div > table > tbody";
    for(let elem of document.querySelector(selector).children){
        var vals=elem.querySelectorAll("p");
        vals[0].innerText
        let += `<entry red="${vals[0].innerText}" green="${vals[1].innerText}" blue="${vals[2].innerText}"/>\n`;
    }

    console.log(file);

})();