#ifndef PAGE_H
#define PAGE_H

const char* rootPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
        }
        .container {
            text-align: center;
            margin-bottom: 20px;
        }
        input[type='text'], input[type='password'] {
            width: 200px;
            padding: 10px;
            margin-bottom: 10px;
            font-size: 16px;
        }
        input[type='submit'], input[type='button'] {
            background-color: #4CAF50;
            color: white;
            padding: 14px 20px;
            margin: 8px 0;
            border: none;
            cursor: pointer;
            font-size: 16px;
            border-radius: 12px; /* Rounded corners */
        }
        input[type='submit']:hover, input[type='button']:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Enter Wi-Fi Credentials</h1>
        <form method='post' action='/add-wifi' id='wifiForm'>
            SSID: <input type='text' name='ssid'><br>
            Password: <input type='password' name='password'><br>
            <input type='submit' value='Connect to Wifi'>
        </form>
    </div>
    <div class="container">
        <input type='button' value='Continue without Wifi' onclick='skipWifi()'>
    </div>

    <script>
        document.getElementById('wifiForm').onsubmit = function(event) {
            event.preventDefault();
            var formData = new FormData(this);
            
            fetch('/add-wifi', {
                method: 'POST',
                body: formData
            })
            .then(response => {
                if (response.ok) {
                    return response.text();
                }
                throw new Error('Network response was not ok.');
            })
            .then(data => {
                alert(data);
            })
            .catch(error => {
                console.error('Error:', error);
            });
        };

        function skipWifi() {
            fetch('/skip-wifi', {
                method: 'POST'
            })
            .then(response => {
                if (response.ok) {
                    return response.text();
                }
                throw new Error('Network response was not ok.');
            })
            .then(data => {
                alert(data);
            })
            .catch(error => {
                console.error('Error:', error);
            });
        }
    </script>
</body>
</html>
)HTML";

const char* controllerPage = R"HTML(
<!DOCTYPE html>
<html lang="en">

    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Color Picker</title>
        <style>
            #s_color_picker_id {
                text-align: center;
            }
        </style>
    </head>

    <body>
        <section id="s_color_picker_id">
            <canvas id="color_picker" width="200" height="200"></canvas>
            <div class="effect-buttons">
                <button class="effect-button" onclick="sendEffectToAPI('RAINBOW_CYCLE')">RAINBOW_CYCLE</button>
                <button class="effect-button" onclick="sendEffectToAPI('RUNNING_LIGHTS')">RUNNING_LIGHTS</button>
                <button class="effect-button" onclick="sendEffectToAPI('THEATER_CHASE')">THEATER_CHASE</button>
                <button class="effect-button" onclick="sendEffectToAPI('AUDIO_EFFECT')">AUDIO_EFFECT</button>
                <button class="effect-button" onclick="sendEffectToAPI('NO_EFFECT')">NO_EFFECT</button>
            </div>
            <div class="effect-buttons">
                <button class="On/Off" onclick="sendTurnLedOnOff()">On/Off</button>
            </div>
            <div class="effect-buttons">
                <input
                    type="range"
                    id="brightness-slider"
                    min="1"
                    max="100"
                    value="100"
                    onchange="sendBrightnessToAPI(this.value)">
            </div>
        </section>

        <script>
            color_picker = document.getElementById("color_picker");
            color_picker.onmousedown = select_color;
            color_picker_add();

            function color_picker_add() {
                color_picker_ = color_picker.getContext("2d"),
                center_x = (color_picker.width) / 2,
                center_y = (color_picker.height) / 2,
                sx = center_x,
                sy = center_y;
                palette = new color_picker_element(center_x, center_y, sx, sy);
                palette.draw();
            }

            function select_color(e) {
                var x = e.pageX - color_picker.offsetLeft,
                    y = e.pageY - color_picker.offsetTop,
                    pixel = color_picker
                        .getContext("2d")
                        .getImageData(x, y, 1, 1)
                        .data,
                    pixelColor = "rgb(" + pixel[0] + ", " + pixel[1] + ", " + pixel[2] + ")";
                sendColorToAPI(pixelColor);
            }

            function sendEffectToAPI(effect) {
                var formData = new FormData();
                formData.append('effect', effect);

                fetch('/setEffect', {
                    method: 'POST',
                    body: formData
                })
                    .then(response => response.text()) // Parse response as text
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            function sendTurnLedOnOff() {
                fetch('/on_off', {method: 'POST'})
                    .then(response => response.text()) // Parse response as text
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            function sendColorToAPI(color) {
                var formData = new FormData();
                formData.append('color', color);

                fetch('/setColor', {
                    method: 'POST',
                    body: formData
                })
                    .then(response => response.text()) // Parse response as text
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            function sendBrightnessToAPI(brightnessPercentage) {
                // Convert percentage (0-100) to range (0-255)
                var brightnessValue = Math.round((brightnessPercentage / 100) * 255);

                var formData = new FormData();
                formData.append('brightness', brightnessValue);

                fetch('/brightness', {
                    method: 'POST',
                    body: formData
                })
                    .then(response => response.text())
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            function color_picker_element(center_x, center_y, sx, sy) {
                this.center_x = center_x;
                this.center_y = center_y;
                this.sx = sx;
                this.sy = sy;
                this.draw = function () {
                    for (var i = 0; i < 360; i += 0.1) {
                        var rad = (i - 45) * (Math.PI) / 180;
                        color_picker_.strokeStyle = "hsla(" + i + ", 100%, 50%, 1.0)";
                        color_picker_.beginPath();
                        color_picker_.moveTo(center_x, center_y);
                        color_picker_.lineTo(
                            center_x + sx * Math.cos(-rad),
                            center_y + sy * Math.sin(-rad)
                        );
                        color_picker_.stroke();
                    }
                }
            }
        </script>
    </body>

</html>
)HTML";

#endif // PAGE_H