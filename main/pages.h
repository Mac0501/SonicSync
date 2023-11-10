#ifndef PAGE_H
#define PAGE_H

const char* rootPage = R"HTML(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>SonicSync</title>
        <style>
            body {
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                height: 100vh;
                margin: 0;
                background-color: #212121;
            }
            #form {
                display: grid;
                place-items: center;
                width: 300px;
                height: 472px;
                padding: 25px;
                background-color: #161616;
                box-shadow: 0 15px 60px #FF69B4;
                outline: 1px solid #ff0396;
                border-radius: 10px;
            }

            #form-body {
                position: absolute;
                top: 50%;
                right: 25px;
                left: 25px;
                width: 230px;
                margin: -156px auto 0;
            }

            #welcome-lines {
                text-align: center;
                line-height: 1;
            }

            #welcome-line-1 {
                color: #FF69B4;
                font-weight: 600;
                font-size: 40px;
            }

            #welcome-line-2 {
                color: #ffffff;
                font-size: 18px;
                margin-top: 17px;
            }

            #input-area {
                margin-top: 10px;
            }

            .form-inp {
                padding: 11px 25px;
                background: transparent;
                border: 1px solid #e3e3e3;
                line-height: 1;
                border-radius: 8px;
            }

            .form-inp:focus {
                border: 1px solid #FF69B4;
            }

            .form-inp:first-child {
                margin-bottom: 15px;
            }

            .form-inp input {
                width: 100%;
                background: none;
                font-size: 13.4px;
                color: #FF69B4;
                border: none;
                padding: 0;
                margin: 0;
            }

            #loader {
                border: 2px solid rgba(255, 255, 255, 0.3);
                border-radius: 50%;
                border-top: 2px solid #FF69B4;
                width: 15px;
                height: 15px;
                animation: spin 2s linear infinite;
                display: inline-block;
                margin-right: 10px;
            }

            @keyframes spin {
                0% {
                    transform: rotate(0deg);
                }
                100% {
                    transform: rotate(360deg);
                }
            }

            .form-inp input:focus {
                outline: none;
            }

            #submit-button-cvr {
                margin-top: 20px;
            }

            #submit-button {
                display: flex;
                /* Use flexbox to align items horizontally */
                align-items: center;
                /* Center items vertically within the button */
                justify-content: center;
                /* Center items horizontally within the button */
                width: 100%;
                height: 40px;
                color: #FF69B4;
                background-color: transparent;
                font-weight: 600;
                font-size: 14px;
                margin: 0;
                padding: 14px 13px 12px;
                border: 0;
                outline: 1px solid #FF69B4;
                border-radius: 8px;
                line-height: 1;
                cursor: pointer;
                transition: all ease-in-out 0.3s;
            }

            #submit-button:hover {
                transition: all ease-in-out 0.3s;
                background-color: #FF69B4;
                color: #161616;
                cursor: pointer;
            }

            #forgot-pass {
                text-align: center;
                margin-top: 10px;
            }

            #forgot-pass a {
                color: #868686;
                font-size: 12px;
                text-decoration: none;
            }

            #bar {
                position: absolute;
                left: 50%;
                bottom: -50px;
                transform: translateX(-50%);
                align-items: center;
                justify-content: center;
                display: flex;
                width: 40px;
                height: 8px;
                margin-left: 0;
                background-color: #FF69B4;
                border-radius: 10px;
            }

            .warning-message {
                display: block;
                color: #161616;
                margin-top: 40px;
                font-size: 12px;
            }

            .show-warning {
                color: red;
            }
            #submit-button.loading:hover {
                background-color: transparent;
                color: #FF69B4;
                cursor: default;
            }
            #confirmation-message {
                color: #ffffff;
                font-size: 15px;
            }
            #timer {
                margin-top: 5px;
                margin-bottom: 5px;
                font-size: 30px;
                text-align: center;
            }
        </style>
    </head>
    <body>
        <div id="form-ui">
            <form method='post' action='/add-wifi' id='form'>
                <div id="form-body">
                    <div id="welcome-lines">
                        <div id="welcome-line-1">SonicSync</div>
                        <div id="welcome-line-2">The Future of light effects</div>
                    </div>
                    <div class="warning-message" id="warningMessage">Incorrect Wlan SSID or Password.</div>
                    <div id="input-area">
                        <div class="form-inp">
                            <input required="" placeholder="Wlan SSID" type="text" name='ssid'>
                        </div>
                        <div class="form-inp">
                            <input required="" placeholder="Wlan Password" type="password" name='password'>
                        </div>
                    </div>
                    <div id="submit-button-cvr">
                        <button id="submit-button" type="submit">Connect to Wlan</button>
                    </div>
                    <div id="forgot-pass">
                        <a onclick='skipWifi()' style="cursor: pointer;">Continue without Wlan</a>
                    </div>
                    <div id="bar"></div>
                </div>
            </form>
        </div>

        <script>
            document
                .getElementById('form')
                .onsubmit = function (event) {
                    event.preventDefault();
                    var formData = new FormData(this);
                    var submitButton = document.getElementById('submit-button');
                    var originalButtonContent = submitButton.innerHTML;

                    submitButton
                        .classList
                        .add('loading');

                    // Change button content to the loading SVG and text
                    submitButton.innerHTML = `
                        <div id="loader" class="loader"></div>
                        Connecting...
                    `;
                    fetch('/add-wifi', {
                        method: 'POST',
                        body: formData
                    })
                        .then(response => {
                            console.log('Response Status:', response.status);
                            console.log('Response Status:', response.content);
                            return response.text();
                        })
                        .then(data => {
                            console.log('Response Content:', data.content);
                            if (response.ok) {
                                var confirmationMessage = document.getElementById('form-ui')
                                confirmationMessage.innerHTML = `
                                <div id="form">
                                    <div id="form-body">
                                        <div id="welcome-lines">
                                            <div id="welcome-line-1">SonicSync</div>
                                            <div id="welcome-line-2">The Future of light effects</div>
                                        </div>
                                        <div id="input-area" style="margin-top: 70px;">
                                            <div id="confirmation-message">
                                                <div>SonicSync is now connected to your network.</div>
                                                <div>You will be redirected in:</div>
                                                <div id="timer">10</div>
                                                <div>Or you can connect now to SonicSync over "${response.content}" in your network.</div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                `;

                                // Implement redirection timer
                                var timerElement = document.getElementById('timer');
                                var count = 10;
                                var timer = setInterval(function () {
                                    count--;
                                    timerElement.textContent = count;
                                    if (count <= 0) {
                                        clearInterval(timer);
                                        // Redirect to the specified URL
                                        window.location.href = response.content;
                                    }
                                }, 1000);
                            } else {
                                var warningMessage = document.getElementById('warningMessage');
                                warningMessage
                                    .classList
                                    .add('show-warning');
                            }
                        })
                        .catch(error => {
                            console.error('Error:', error);
                        })
                        . finally(() => {
                            submitButton
                                .classList
                                .remove('loading');
                            // Restore the original button content after fetch completes
                            submitButton.innerHTML = originalButtonContent;
                        });
                };

            function skipWifi() {
                fetch('/skip-wifi', {method: 'POST'})
                    .then(response => {
                        if (response.ok) {
                            // Redirect to the /controller page
                            window.location.href = '/controller';
                        } else {
                            throw new Error('Network response was not ok.');
                        }
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