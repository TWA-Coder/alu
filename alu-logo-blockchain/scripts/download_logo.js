const https = require('https');
const fs = require('fs');

https.get('https://www.alueducation.com/wp-content/uploads/2021/02/ALU-Logo.png', (res) => {
    res.pipe(fs.createWriteStream('alu-logo.png'));
    console.log("Logo downloaded.");
}).on('error', (e) => {
    console.error(e);
});
