const char index_page[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Antrian View</title>
      <link rel="stylesheet" href="style.css">
  </head>
  <body>
      <div class="header">
          <h2>Sistem Antrian</h2>
          <p id="jam">12 : 00</p>
      </div>
      <div class="content">
  
          <div class="box">
              <div class="header-box">
                  <h1>CS</h1>
              </div>
              <div class="isi">
                  <div class="huruf">
                      <h1>A -</h1>
                  </div>
                  <div class="huruf">
                      <h1 id="antri-a">1</h1>
                  </div>
              </div>
          </div>
          <div class="box">
              <div class="header-box">
                  <h1>Teler</h1>
              </div>
              <div class="isi">
                  <div class="huruf">
                      <h1>B -</h1>
                  </div>
                  <div class="huruf">
                      <h1 id="antri-b">1</h1>
                  </div>
              </div>
          </div>

          // <script src="main.js"></script>
      </div>
      <script>
        setInterval(jam, 1000);
        setInterval(data, 250);

        function jam(){
            let date = new Date();
            let jam = date.getHours();
            let menit = date.getMinutes();
            if (menit < 10){
                menit = '0' + menit;
            }
            document.getElementById("jam").innerHTML = jam + " : " + menit;
        }
    
        function data(){
            fetch(`/update_view`)
           .then(response => response.json())
           .then(data => {
            document.getElementById("antri-a").innerHTML = data.antriA;
            document.getElementById("antri-b").innerHTML = data.antriB;
           })
        }
      </script>
  </body>
  </body>
  </html>


)rawliteral";



const char antri_a[] PROGMEM = R"rawliteral(

  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Antri-b</title>
      <!-- <link rel="stylesheet" href="style.css"> -->
      <style>
          body {
              padding: 0;
              margin: 0;
              font-family: Arial, Helvetica, sans-serif;
          }

          .header {
              background-color: #333;
              color: #fff;
              padding: 10px 20px 10px 20px; 
              display: flex;
              justify-content: space-between;
              align-items: center;
              box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          }

          .header p {
              font-size: 20px;
              font-weight: bold;
          }

          .content {
              width: 100% !important;
              margin: 20px 0;
              display: flex;

              /* gap: 2rem; */
              display: flex;
              justify-content: center;
          }

          .box {
              width: 100%;
              border: #333 2px solid;
          }

          .header-box {
              background-color: #46e6ff;
              display: flex;
              justify-content: center;
              align-items: center;
          }

          .header-box h1 {
              font-size: 50px;
              color: #fff;
          }
          .isi {
              padding: 20px;
              display: flex !important;
              justify-content: center;
              text-align: center;
              background-color: #f9f9f9;
              border-radius: 10px;
          }

          .huruf {
              padding: 10px;
          }

          .huruf h1 {
              font-size: 80px;
              font-weight: bold;
              color: #333;
          }

          .control {
              position: absolute;

              /* background-color: #46e6ff; */
              width: 100%;
              height: 100%;
          }

          .display {
              position: absolute;
              top: 50%;
              left: 50%;
              width: 400px;
              height: 500px;
              border-radius: 25px;
              transform: translate(-50%, -50%);
              font-size: 50px;
              font-weight: bold;
              color: #fff;
              background-color: #494444;
          }


          .display-box {
              display: grid;
              justify-content: center;
              text-align: center;
              margin-top: 2rem;
          }

          .display-box p {
              font-size: 20px;
              margin: px;
          }

          .display-box h4 {
              margin: 0;
          }

          .display-angka {
              margin: 70px 0;
              display: grid;
              justify-content: center;
              align-items: center;
          }

          .display-angka p {
              margin: 5px;
          }



          button {
              padding: 10px 20px;
              border: none;
              border-radius: 5px;
              cursor: pointer;
              transition: background-color 0.3s ease;
          }
      </style>
  </head>
  <body>
      <div class="header">
          <h2>Control Antrian - A</h2>
          <p id="jam">N/A</p>
      </div>
      <div class="control">
          <div class="display">
              <div class="display-box">
                  <p>Antrian saat ini</p>
                  <h4 id="display-a">N/A</h4>
                  <div class="display-angka">
                      <p>Terakhir : <span id="antriterakhir-a">N/A</span></p>
                      <p>Sisa : <span id="antrisisa-a">N/A</span></p>
                  </div>
                  <div class="display-button">
                      <button onclick="prev('a')">Prev</button>
                      <button onclick="reset('a')">Reset</button>
                      <button onclick="next('a')">Next</button>
                  </div>
              </div>
          </div>
      </div>
      <script>
        let antriA = 0;
        let jumlahA = 0;
        let sisaA = 0;

        setInterval(updateJam, 1000);
        setInterval(update, 200);

        // window.onload = function(){
        //     update();
        // }

        function updateJam(){
            let jam = new Date().toLocaleTimeString();
            document.getElementById("jam").innerHTML = jam;
        }

        function update(){
            fetch(`/update_a`)
            .then(response => response.json())
            .then(data => {
                antriA = data.antriA;
                jumlahA = data.jumlahA;
                sisaA = data.sisaA;
                console.log(data);
                display();
            });
        }

        function next(kls) {
          if (antriA < jumlahA){
            fetch(`/next?buttonId=${kls}`)
            .then(response => response.text())
            .then(data => console.log(data));
            update();
          } else {
            alert("Antrian sudah Habis !!!. Silahkan Tunggu.");
          }
        }

        function prev(kls) {
          if (antriA > 0){
            fetch(`/prev?buttonId=${kls}`)
            .then(response => response.text())
            .then(data => console.log(data))
            update();
          }
        }


        function display() {
            document.getElementById("display-a").innerHTML = antriA;
            document.getElementById("antriterakhir-a").innerHTML = jumlahA;
            document.getElementById("antrisisa-a").innerHTML = sisaA;
        }
      </script>
  </body>
  </html>

)rawliteral";


const char antri_b[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Antri-b</title>
      <!-- <link rel="stylesheet" href="style.css"> -->
      <style>
          body {
              padding: 0;
              margin: 0;
              font-family: Arial, Helvetica, sans-serif;
          }

          .header {
              background-color: #333;
              color: #fff;
              padding: 10px 20px 10px 20px; 
              display: flex;
              justify-content: space-between;
              align-items: center;
              box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          }

          .header p {
              font-size: 20px;
              font-weight: bold;
          }

          .content {
              width: 100% !important;
              margin: 20px 0;
              display: flex;

              /* gap: 2rem; */
              display: flex;
              justify-content: center;
          }

          .box {
              width: 100%;
              border: #333 2px solid;
          }

          .header-box {
              background-color: #46e6ff;
              display: flex;
              justify-content: center;
              align-items: center;
          }

          .header-box h1 {
              font-size: 50px;
              color: #fff;
          }
          .isi {
              padding: 20px;
              display: flex !important;
              justify-content: center;
              text-align: center;
              background-color: #f9f9f9;
              border-radius: 10px;
          }

          .huruf {
              padding: 10px;
          }

          .huruf h1 {
              font-size: 80px;
              font-weight: bold;
              color: #333;
          }

          .control {
              position: absolute;

              /* background-color: #46e6ff; */
              width: 100%;
              height: 100%;
          }

          .display {
              position: absolute;
              top: 50%;
              left: 50%;
              width: 400px;
              height: 500px;
              border-radius: 25px;
              transform: translate(-50%, -50%);
              font-size: 50px;
              font-weight: bold;
              color: #fff;
              background-color: #494444;
          }


          .display-box {
              display: grid;
              justify-content: center;
              text-align: center;
              margin-top: 2rem;
          }

          .display-box p {
              font-size: 20px;
              margin: px;
          }

          .display-box h4 {
              margin: 0;
          }

          .display-angka {
              margin: 70px 0;
              display: grid;
              justify-content: center;
              align-items: center;
          }

          .display-angka p {
              margin: 5px;
          }



          button {
              padding: 10px 20px;
              border: none;
              border-radius: 5px;
              cursor: pointer;
              transition: background-color 0.3s ease;
          }
      </style>
  </head>
  <body>
      <div class="header">
          <h2>Control Antrian - B</h2>
          <p id="jam">N/A</p>
      </div>
      <div class="control">
          <div class="display">
              <div class="display-box">
                  <p>Antrian saat ini</p>
                  <h4 id="display-b">N/A</h4>
                  <div class="display-angka">
                      <p>Terakhir : <span id="antriterakhir-b">N/A</span></p>
                      <p>Sisa : <span id="antrisisa-b">N/A</span></p>
                  </div>
                  <div class="display-button">
                      <button onclick="prev('b')">Prev</button>
                      <button onclick="reset('b')">Reset</button>
                      <button onclick="next('b')">Next</button>
                  </div>
              </div>
          </div>
      </div>
      <script>
        let antriB = 0;
        let jumlahB = 0;
        let sisaB = 0;

        setInterval(updateJam, 1000);
        setInterval(update, 200);

        // window.onload = function(){
        //     update();
        // }

        function updateJam(){
            let jam = new Date().toLocaleTimeString();
            document.getElementById("jam").innerHTML = jam;
        }

        function update(){
            fetch(`/update_b`)
            .then(response => response.json())
            .then(data => {
                antriB = data.antriB;
                jumlahB = data.jumlahB;
                sisaB = data.sisaB;
                console.log(data);
                display();
            });
        }

        function next(kls) {
          if (antriB < jumlahB){
            fetch(`/next?buttonId=${kls}`)
            .then(response => response.text())
            .then(data => console.log(data));
            update();
          } else {
            alert("Antrian sudah Habis !!!. Silahkan Tunggu.");
          }
        }

        function prev(kls) {
          if (antriB > 0){
            fetch(`/prev?buttonId=${kls}`)
            .then(response => response.text())
            .then(data => console.log(data))
            update();
          }
        }


        function display() {
            document.getElementById("display-b").innerHTML = antriB;
            document.getElementById("antriterakhir-b").innerHTML = jumlahB;
            document.getElementById("antrisisa-b").innerHTML = sisaB;
        }
      </script>
  </body>
  </html>

)rawliteral";