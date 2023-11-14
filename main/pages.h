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
                align-items: center;
                justify-content: center;
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

                                var timerElement = document.getElementById('timer');
                                var count = 10;
                                var timer = setInterval(function () {
                                    count--;
                                    timerElement.textContent = count;
                                    if (count <= 0) {
                                        clearInterval(timer);
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
                            submitButton.innerHTML = originalButtonContent;
                        });
                };

            function skipWifi() {
                fetch('/skip-wifi', {method: 'POST'})
                    .then(response => {
                        if (response.ok) {
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
        <title>SonicSync</title>
        <style>
            body {
                display: flex;
                flex-direction: column;
                align-items: center;
                height: 100vh;
                margin: 0;
                background-color: #212121;
            }

            .layout {
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                text-align: center;
                margin-top: 50px;
            }

            #color-wheel {
                width: 180px;
                height: 180px;
                background: radial-gradient(white, transparent 80%), conic-gradient(#e43f00, #fae410, #55cc3b, #09adff, #6b0efd, #e70d86, #e43f00);
                border-radius: 50%;
                cursor: pointer;
                position: relative;
            }
            .main {
                display: flex;
                flex-direction: column;
                gap: 0.5em;
                margin-top: 20px;
            }

            .up {
                display: flex;
                flex-direction: row;
                gap: 0.5em;
            }

            .down {
                display: flex;
                flex-direction: row;
                gap: 0.5em;
            }

            .card1 {
                width: 90px;
                height: 90px;
                outline: none;
                border: none;
                background: white;
                border-radius: 90px 5px 5px 5px;
                box-shadow: rgba(50, 50, 93, 0.25) 0 2px 5px -1px, rgba(0, 0, 0, 0.3) 0 1px 3px -1px;
                transition: 0.2s ease-in-out;
                display: flex;
                justify-content: center;
                align-items: center;

            }

            .card1 svg {
                height: 50px;
                width: 50px;
                z-index: 10;
                fill: #cc39a4;
                margin: 15px 0 0 15px;
            }

            .card2 {
                width: 90px;
                height: 90px;
                outline: none;
                border: none;
                background: white;
                border-radius: 5px 90px 5px 5px;
                box-shadow: rgba(50, 50, 93, 0.25) 0 2px 5px -1px, rgba(0, 0, 0, 0.3) 0 1px 3px -1px;
                transition: 0.2s ease-in-out;
                display: flex;
                justify-content: center;
                align-items: center;
            }

            .card2 svg {
                height: 50px;
                width: 50px;
                z-index: 10;
                fill: #cc39a4;
                margin: 15px 15px 0 0;
            }

            .card3 {
                width: 90px;
                height: 90px;
                outline: none;
                border: none;
                background: white;
                border-radius: 5px 5px 5px 90px;
                box-shadow: rgba(50, 50, 93, 0.25) 0 2px 5px -1px, rgba(0, 0, 0, 0.3) 0 1px 3px -1px;
                transition: 0.2s ease-in-out;
                display: flex;
                justify-content: center;
                align-items: center;
            }

            .card3 svg {
                height: 50px;
                width: 50px;
                z-index: 10;
                fill: #cc39a4;
                margin: 0 0 15px 15px;
            }

            .card4 {
                width: 90px;
                height: 90px;
                outline: none;
                border: none;
                background: white;
                border-radius: 5px 5px 90px 5px;
                box-shadow: rgba(50, 50, 93, 0.25) 0 2px 5px -1px, rgba(0, 0, 0, 0.3) 0 1px 3px -1px;
                transition: 0.2s ease-in-out;
                display: flex;
                justify-content: center;
                align-items: center;
            }

            .card4 svg {
                height: 50px;
                width: 50px;
                z-index: 10;
                fill: #cc39a4;
                margin: 0 15px 15px 0;
            }

            .card1:hover {
                cursor: pointer;
                scale: 1.1;
                background-color: #cc39a4;
            }

            .card2:hover {
                cursor: pointer;
                scale: 1.1;
                background-color: #03A9F4;
            }

            .card3:hover {
                cursor: pointer;
                scale: 1.1;
                background-color: rgb(18, 206, 37);
            }

            .card4:hover {
                cursor: pointer;
                scale: 1.1;
                background-color: #8c9eff;
            }

            .delete-button {
                width: 140px;
                height: 50px;
                border-radius: 50px;
                background-color: rgb(20, 20, 20);
                border: none;
                font-weight: 600;
                display: flex;
                align-items: center;
                justify-content: space-evenly;
                box-shadow: 0 0 20px rgba(0, 0, 0, 0.164);
                cursor: pointer;
                overflow: hidden;
                position: relative;
                content: "Delete";
                color: white;
                font-size: 13px;
                transition: 0.3s;
                margin-top: 10px;
            }
            .delete-button:active {
                background-color: rgb(255, 69, 69);
            }
            .delete-svgIcon {
                width: 12px;
            }

            .delete-svgIcon path {
                fill: white;
            }

            .on-button {
                fill: #fff;
                background-color: #ffffff11;
                padding: 10px;
                border-radius: 50%;
                border: 1px solid #fff1;
                width: 30px;
                height: 30px;
                filter: drop-shadow(0px 0px 10px rgb(255, 255, 255));
                transition: 0.2s;
                cursor: pointer;
                margin-left: 5vw;
            }

            .on-button:active {
                filter: drop-shadow(0px 0px 10px rgb(255, 255, 255)) brightness(50%);
            }

            .options-button {
                fill: #fff;
                background-color: #ffffff11;
                padding: 10px;
                border-radius: 200vw;
                border: 1px solid #fff1;
                width: 30px;
                height: 30px;
                filter: drop-shadow(0px 0px 10px rgb(255, 255, 255));
                cursor: pointer;
                margin-right: 5vw;
            }

            .header-layout {
                display: flex;
                justify-content: space-between;
                align-items: center;
                text-align: center;
                margin-top: 5vw;
                max-height: 30px;
                width: 500px;
                max-width: 100vw;
            }

            #crosshair {
                position: absolute;
                width: 2px;
                height: 14px;
                background-color: black;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
            }

            #crosshair::after,
            #crosshair::before {
                content: "";
                position: absolute;
                width: 14px;
                height: 2px;
                background-color: black;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
            }

            #crosshair::before {
                transform: translate(-50%, -50%) rotate(90deg);
            }

            .slider {
                cursor: pointer;
                display: -webkit-inline-box;
                display: -ms-inline-flexbox;
                display: flex;
                justify-content: space-between;
                -webkit-box-orient: horizontal;
                -webkit-box-direction: reverse;
                -ms-flex-direction: row-reverse;
                flex-direction: row-reverse;
                -webkit-box-align: center;
                -ms-flex-align: center;
                align-items: center;
                margin-top: 10px;
            }

            .slider .volume {
                display: inline-block;
                vertical-align: top;
                margin-right: 15px;
                color: rgb(121, 120, 120);
                height: auto;
            }

            .slider .percentage {
                display: inline-block;
                vertical-align: top;
                margin-left: 10px;
                min-width: 38px;
                color: rgb(121, 120, 120);
                height: auto;
            }

            .slider .level {
                -webkit-appearance: none;
                -moz-appearance: none;
                appearance: none;
                width: 100%;
                height: 6px;
                background: rgb(121, 120, 120);
                overflow: hidden;
                border-radius: 999px;
                -webkit-transition: height 0.1s;
                -o-transition: height 0.1s;
                transition: height 0.1s;
                cursor: inherit;
            }

            .slider .level::-webkit-slider-thumb {
                -webkit-appearance: none;
                width: 0;
                height: 0;
                -webkit-box-shadow: -200px 0 0 200px #fff;
                box-shadow: -200px 0 0 200px #fff;
            }

            .slider:hover .level {
                height: calc(6px * 2);
            }

            .settings-card {
                display: none;
                position: fixed;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
                max-width: 320px;
                padding: 1rem;
                background-color: #fff;
                border-radius: 10px;
                box-shadow: 20px 20px 30px rgba(0, 0, 0, .05);
                z-index: 1000;
            }

            .title {
                font-weight: 600;
                color: rgb(31 41 55);
            }

            .description {
                margin-top: 1rem;
                font-size: 0.875rem;
                line-height: 1.25rem;
                color: rgb(75 85 99);
            }

            .description a {
                --tw-text-opacity: 1;
                color: rgb(59 130 246);
            }

            .description a:hover {
                -webkit-text-decoration-line: underline;
                text-decoration-line: underline;
            }

            .actions {
                display: flex;
                align-items: center;
                justify-content: space-between;
                margin-top: 1rem;
                -moz-column-gap: 1rem;
                column-gap: 1rem;
                flex-shrink: 0;
            }

            .pref {
                font-size: 0.75rem;
                line-height: 1rem;
                color: rgb(31 41 55);
                -webkit-text-decoration-line: underline;
                text-decoration-line: underline;
                transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
                border: none;
                background-color: transparent;
            }

            .pref:hover {
                color: rgb(156 163 175);
            }

            .pref:focus {
                outline: 2px solid transparent;
                outline-offset: 2px;
            }

            .accept {
                font-size: 0.75rem;
                line-height: 1rem;
                background-color: #4CAF50;
                font-weight: 500;
                border-radius: 0.5rem;
                color: #fff;
                padding: 0.625rem 1rem;
                border: none;
                transition: all 0.15s cubic-bezier(0.4, 0, 0.2, 1);
            }

            .accept:hover {
                background-color: #45a049;
            }

            .accept:focus {
                outline: 2px solid transparent;
                outline-offset: 2px;
            }

            .cancel {
                font-size: 0.75rem;
                line-height: 1rem;
                background-color: #FF6961;
                font-weight: 500;
                border-radius: 0.5rem;
                color: #fff;
                padding: 0.625rem 1rem;
                border: none;
                transition: all 0.15s cubic-bezier(0.4, 0, 0.2, 1);
            }

            .cancel:hover {
                background-color: #E2574C;
            }

            .cancel:focus {
                outline: 2px solid transparent;
                outline-offset: 2px;
            }

            .overlay {
                position: fixed;
                top: 0;
                left: 0;
                width: 100%;
                height: 100%;
                background: rgba(0, 0, 0, 0.5);
                display: none;
                z-index: 1000;
            }

            body.overlay-active {
                overflow: hidden;
            }
        </style>
    </head>

    <body>
        <div class="header-layout">
            <div class="on-button" onclick="sendTurnLedOnOff()">
                <svg
                    height="100%"
                    width="100%"
                    version="1.1"
                    id="Capa_1"
                    xmlns="http://www.w3.org/2000/svg"
                    xmlns:xlink="http://www.w3.org/1999/xlink"
                    viewbox="0 0 30.143 30.143"
                    xml:space="preserve">
                    <g>
                        <path
                            style="fill:'currentColor';"
                            d="M20.034,2.357v3.824c3.482,1.798,5.869,5.427,5.869,9.619c0,5.98-4.848,10.83-10.828,10.83
                            c-5.982,0-10.832-4.85-10.832-10.83c0-3.844,2.012-7.215,5.029-9.136V2.689C4.245,4.918,0.731,9.945,0.731,15.801
                            c0,7.921,6.42,14.342,14.34,14.342c7.924,0,14.342-6.421,14.342-14.342C29.412,9.624,25.501,4.379,20.034,2.357z"/>
                        <path
                            style="fill:'currentColor';"
                            d="M14.795,17.652c1.576,0,1.736-0.931,1.736-2.076V2.08c0-1.148-0.16-2.08-1.736-2.08
		                    c-1.57,0-1.732,0.932-1.732,2.08v13.496C13.062,16.722,13.225,17.652,14.795,17.652z"/>
                    </g>
                </svg>
            </div>

            <div class="options-button" onclick="showSettingsCard()">
                <svg
                    xmlns="http://www.w3.org/2000/svg"
                    x="0px"
                    y="0px"
                    height="100%"
                    width="100%"
                    viewbox="0 0 32 32">
                    <path
                        d="M 13.1875 3 L 13.03125 3.8125 L 12.4375 6.78125 C 11.484375 7.15625 10.625 7.683594 9.84375 8.3125 L 6.9375 7.3125 L 6.15625 7.0625 L 5.75 7.78125 L 3.75 11.21875 L 3.34375 11.9375 L 3.9375 12.46875 L 6.1875 14.4375 C 6.105469 14.949219 6 15.460938 6 16 C 6 16.539063 6.105469 17.050781 6.1875 17.5625 L 3.9375 19.53125 L 3.34375 20.0625 L 3.75 20.78125 L 5.75 24.21875 L 6.15625 24.9375 L 6.9375 24.6875 L 9.84375 23.6875 C 10.625 24.316406 11.484375 24.84375 12.4375 25.21875 L 13.03125 28.1875 L 13.1875 29 L 18.8125 29 L 18.96875 28.1875 L 19.5625 25.21875 C 20.515625 24.84375 21.375 24.316406 22.15625 23.6875 L 25.0625 24.6875 L 25.84375 24.9375 L 26.25 24.21875 L 28.25 20.78125 L 28.65625 20.0625 L 28.0625 19.53125 L 25.8125 17.5625 C 25.894531 17.050781 26 16.539063 26 16 C 26 15.460938 25.894531 14.949219 25.8125 14.4375 L 28.0625 12.46875 L 28.65625 11.9375 L 28.25 11.21875 L 26.25 7.78125 L 25.84375 7.0625 L 25.0625 7.3125 L 22.15625 8.3125 C 21.375 7.683594 20.515625 7.15625 19.5625 6.78125 L 18.96875 3.8125 L 18.8125 3 Z M 14.8125 5 L 17.1875 5 L 17.6875 7.59375 L 17.8125 8.1875 L 18.375 8.375 C 19.511719 8.730469 20.542969 9.332031 21.40625 10.125 L 21.84375 10.53125 L 22.40625 10.34375 L 24.9375 9.46875 L 26.125 11.5 L 24.125 13.28125 L 23.65625 13.65625 L 23.8125 14.25 C 23.941406 14.820313 24 15.402344 24 16 C 24 16.597656 23.941406 17.179688 23.8125 17.75 L 23.6875 18.34375 L 24.125 18.71875 L 26.125 20.5 L 24.9375 22.53125 L 22.40625 21.65625 L 21.84375 21.46875 L 21.40625 21.875 C 20.542969 22.667969 19.511719 23.269531 18.375 23.625 L 17.8125 23.8125 L 17.6875 24.40625 L 17.1875 27 L 14.8125 27 L 14.3125 24.40625 L 14.1875 23.8125 L 13.625 23.625 C 12.488281 23.269531 11.457031 22.667969 10.59375 21.875 L 10.15625 21.46875 L 9.59375 21.65625 L 7.0625 22.53125 L 5.875 20.5 L 7.875 18.71875 L 8.34375 18.34375 L 8.1875 17.75 C 8.058594 17.179688 8 16.597656 8 16 C 8 15.402344 8.058594 14.820313 8.1875 14.25 L 8.34375 13.65625 L 7.875 13.28125 L 5.875 11.5 L 7.0625 9.46875 L 9.59375 10.34375 L 10.15625 10.53125 L 10.59375 10.125 C 11.457031 9.332031 12.488281 8.730469 13.625 8.375 L 14.1875 8.1875 L 14.3125 7.59375 Z M 16 11 C 13.25 11 11 13.25 11 16 C 11 18.75 13.25 21 16 21 C 18.75 21 21 18.75 21 16 C 21 13.25 18.75 11 16 11 Z M 16 13 C 17.667969 13 19 14.332031 19 16 C 19 17.667969 17.667969 19 16 19 C 14.332031 19 13 17.667969 13 16 C 13 14.332031 14.332031 13 16 13 Z"></path>
                </svg>
            </div>

        </div>
        <div class="layout">
            <div id="color-wheel">
                <div id="crosshair"></div>
            </div>
            <div class="main">
                <div class="up">
                    <button class="card1" onclick="sendEffectToAPI('RAINBOW_CYCLE')">
                        <?xml version="1.0" standalone="no"?>
                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 20010904//EN" "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
                        <svg
                            version="1.0"
                            xmlns="http://www.w3.org/2000/svg"
                            width="300.000000pt"
                            height="300.000000pt"
                            viewbox="0 0 300.000000 300.000000"
                            preserveaspectratio="xMidYMid meet">
                            <g
                                transform="translate(0.000000,300.000000) scale(0.100000,-0.100000)"
                                fill="#000000"
                                stroke="none">
                                <path
                                    d="M1400 2083 c-228 -36 -412 -122 -562 -262 -137 -129 -229 -281 -274
                                    -458 -20 -78 -26 -133 -31 -271 -6 -157 -4 -174 12 -192 20 -23 57 -26 73 -8
                                    5 7 12 94 15 193 3 126 10 199 22 245 21 79 81 211 126 277 110 160 288 286
                                    496 350 79 24 104 27 243 27 121 1 170 -3 224 -18 314 -85 545 -293 638 -576
                                    30 -90 32 -105 36 -285 2 -104 8 -198 13 -207 12 -23 52 -23 74 -1 15 15 17
                                    38 13 207 -4 171 -8 200 -32 281 -106 355 -388 609 -761 686 -83 17 -253 24
                                    -325 12z"/>
                                <path
                                    d="M1360 1870 c-259 -56 -464 -215 -571 -443 -51 -110 -60 -159 -66
                                    -344 -6 -184 -2 -203 41 -203 48 1 51 8 57 197 5 147 10 188 28 238 50 138
                                    160 276 287 360 86 58 231 104 346 112 329 21 652 -202 733 -507 10 -36 15
                                    -105 15 -202 0 -172 7 -198 49 -198 20 0 34 7 41 19 15 30 12 304 -5 386 -24
                                    114 -94 244 -183 337 -208 217 -492 309 -772 248z"/>
                                <path
                                    d="M1421 1624 c-133 -36 -232 -117 -293 -242 l-33 -66 -3 -204 c-4 -215
                                    -1 -232 42 -232 42 0 46 15 46 186 0 134 3 173 20 225 49 157 208 264 365 245
                                    111 -13 223 -95 276 -204 22 -43 24 -62 27 -237 2 -112 8 -195 14 -202 15 -19
                                    52 -16 71 5 14 16 17 43 17 174 0 177 -9 236 -46 313 -94 191 -306 292 -503
                                    239z"/>
                                <path
                                    d="M967 1420 c-48 -38 -60 -96 -65 -306 -4 -207 1 -234 43 -234 51 0 55
                                    16 55 213 0 173 1 185 25 235 30 65 30 67 5 92 -25 25 -31 25 -63 0z"/>
                                <path
                                    d="M1458 1426 c-81 -22 -143 -82 -173 -167 -19 -54 -22 -348 -3 -367 19
                                    -19 55 -14 72 10 13 18 16 52 16 166 0 161 12 200 72 244 44 31 124 32 164 2
                                    70 -52 83 -96 84 -280 0 -136 5 -154 45 -154 41 0 45 16 45 189 0 153 -2 170
                                    -24 217 -29 63 -95 120 -160 139 -59 18 -76 18 -138 1z"/>
                                <path
                                    d="M2028 1429 c-23 -13 -23 -47 2 -100 17 -39 20 -66 20 -227 0 -107 4
                                    -191 10 -203 15 -27 66 -26 80 2 8 14 10 88 8 222 -3 197 -4 203 -30 253 -32
                                    62 -54 74 -90 53z"/>
                            </g>
                        </svg>
                    </button>
                    <button class="card2" onclick="sendEffectToAPI('AUDIO_EFFECT')">
                        <?xml version="1.0" standalone="no"?>
                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 20010904//EN" "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
                        <svg
                            version="1.0"
                            xmlns="http://www.w3.org/2000/svg"
                            width="300.000000pt"
                            height="300.000000pt"
                            viewbox="0 0 300.000000 300.000000"
                            preserveaspectratio="xMidYMid meet">
                            <g
                                transform="translate(0.000000,300.000000) scale(0.100000,-0.100000)"
                                fill="#000000"
                                stroke="none">
                                <path
                                    d="M846 2314 c-14 -14 -16 -102 -16 -824 l0 -810 25 -16 c29 -19 102
                                    -14 116 8 5 7 9 378 9 825 0 793 0 813 -19 823 -30 15 -97 12 -115 -6z"/>
                                <path
                                    d="M1136 2314 c-14 -14 -16 -80 -16 -579 l0 -564 37 24 c20 14 53 28 75
                                    31 l38 7 0 538 c0 522 -1 539 -19 549 -30 15 -97 12 -115 -6z"/>
                                <path
                                    d="M1428 2319 c-16 -9 -18 -27 -18 -154 0 -94 4 -145 10 -145 6 0 40 7
                                    75 16 l65 16 0 129 c0 117 -2 129 -19 139 -25 13 -91 13 -113 -1z"/>
                                <path
                                    d="M1718 2319 c-15 -9 -18 -26 -18 -120 l0 -109 28 7 c15 3 47 12 72 18
                                    l45 11 3 86 c2 68 -1 89 -13 102 -19 18 -90 21 -117 5z"/>
                                <path
                                    d="M1997 2312 c-12 -13 -17 -37 -17 -86 0 -59 2 -67 18 -62 9 3 37 10
                                    62 16 84 21 80 17 80 70 0 63 -17 80 -79 80 -34 0 -53 -5 -64 -18z"/>
                                <path
                                    d="M2050 2076 c-69 -19 -239 -63 -379 -99 -140 -37 -258 -72 -263 -79
                                    -4 -7 -7 -188 -6 -401 3 -384 3 -388 -17 -382 -77 24 -94 26 -132 16 -80 -22
                                    -133 -91 -133 -176 0 -133 118 -213 254 -173 52 15 133 95 166 163 25 49 25
                                    54 30 389 l5 340 219 57 c120 32 231 61 247 64 l29 7 0 -351 c0 -193 -2 -351
                                    -5 -351 -2 0 -19 9 -37 20 -23 14 -50 20 -88 20 -192 -1 -248 -246 -77 -337
                                    46 -24 132 -22 182 4 55 29 118 97 152 163 l28 55 0 540 0 540 -25 2 c-14 1
                                    -81 -13 -150 -31z"/>
                                <path
                                    d="M1770 1621 l-65 -17 -5 -129 c-3 -72 -3 -182 -1 -246 l3 -116 40 43
                                    c22 24 55 48 74 54 l34 12 0 209 c0 115 -3 209 -7 208 -5 0 -37 -9 -73 -18z"/>
                                <path
                                    d="M1697 765 c-10 -96 16 -125 100 -112 49 8 71 41 36 54 -37 14 -93 57
                                    -111 85 l-19 30 -6 -57z"/>
                                <path
                                    d="M1522 767 c-19 -19 -52 -43 -73 -53 -25 -11 -39 -23 -37 -33 4 -21
                                    58 -35 102 -27 41 8 49 26 44 100 l-3 47 -33 -34z"/>
                                <path
                                    d="M2096 729 c-21 -16 -54 -31 -73 -35 -53 -10 -33 -29 34 -32 70 -3 86
                                    9 81 61 l-3 34 -39 -28z"/>
                                <path
                                    d="M1120 711 c0 -40 23 -55 82 -54 68 2 81 17 25 31 -24 6 -55 21 -69
                                    32 -34 26 -38 25 -38 -9z"/>
                            </g>
                        </svg>

                    </button>
                </div>
                <div class="down">
                    <button class="card3" onclick="sendEffectToAPI('THEATER_CHASE')">
                        <?xml version="1.0" standalone="no"?>
                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 20010904//EN" "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
                        <svg
                            version="1.0"
                            xmlns="http://www.w3.org/2000/svg"
                            width="900.000000pt"
                            height="900.000000pt"
                            viewbox="0 0 900.000000 900.000000"
                            preserveaspectratio="xMidYMid meet">
                            <g
                                transform="translate(0.000000,900.000000) scale(0.100000,-0.100000)"
                                fill="#000000"
                                stroke="none">
                                <path
                                    d="M4408 7981 c-31 -20 -58 -54 -76 -94 -19 -44 -17 -983 2 -1028 27
                                    -65 97 -109 171 -109 71 0 146 55 165 121 6 20 10 230 10 514 0 542 3 521 -78
                                    582 -39 29 -50 33 -105 32 -41 0 -71 -6 -89 -18z"/>
                                <path
                                    d="M3455 7227 c-61 -17 -132 -94 -141 -153 -15 -100 10 -164 83 -220 34
                                    -26 46 -29 114 -29 74 0 78 2 123 39 70 57 81 79 81 161 -1 66 -3 75 -35 116
                                    -45 57 -65 71 -120 86 -51 14 -51 14 -105 0z"/>
                                <path
                                    d="M5425 7226 c-51 -14 -117 -81 -133 -134 -30 -100 0 -182 88 -243 28
                                    -19 47 -23 106 -23 61 0 77 4 113 27 64 42 93 94 93 165 0 101 -61 182 -158
                                    210 -47 14 -54 14 -109 -2z"/>
                                <path
                                    d="M6793 6985 c-23 -7 -48 -19 -56 -26 -8 -8 -140 -135 -293 -284 -399
                                    -387 -409 -396 -427 -434 -21 -42 -22 -98 -2 -139 18 -36 76 -89 112 -103 41
                                    -15 111 -10 154 12 23 11 161 136 308 278 146 142 299 289 339 327 40 38 82
                                    83 93 100 54 87 -19 243 -126 268 -16 4 -37 9 -45 11 -8 2 -34 -2 -57 -10z"/>
                                <path
                                    d="M2083 6984 c-13 -3 -43 -29 -68 -57 -45 -51 -45 -52 -45 -126 l0 -75
                                    178 -174 c344 -338 541 -521 581 -541 74 -38 149 -24 219 42 39 37 56 94 47
                                    155 -6 39 -17 55 -88 126 -174 172 -623 605 -650 626 -28 23 -122 35 -174 24z"/>
                                <path
                                    d="M4285 6441 c-172 -17 -407 -91 -595 -188 -148 -76 -359 -236 -473
                                    -357 -54 -57 -178 -217 -223 -286 -59 -92 -130 -231 -157 -306 -100 -279 -120
                                    -388 -120 -663 -1 -158 3 -226 17 -291 48 -235 110 -405 218 -600 87 -159 264
                                    -362 428 -492 l55 -44 5 -350 c4 -277 8 -355 20 -379 27 -57 73 -106 121 -130
                                    l48 -25 876 0 c816 0 877 1 908 18 71 38 110 81 134 152 9 25 13 130 13 365 0
                                    182 4 335 8 341 4 6 36 35 72 64 97 80 213 196 272 275 29 39 55 72 58 75 9 9
                                    46 65 53 80 4 8 16 29 26 45 34 53 107 205 134 280 40 112 43 120 66 225 36
                                    162 41 208 41 385 0 176 -9 264 -41 395 -6 25 -15 61 -19 80 -8 33 -15 55 -47
                                    145 -23 64 -92 212 -126 270 -56 96 -78 130 -130 200 -54 72 -237 262 -312
                                    325 -26 21 -169 119 -231 157 -33 21 -217 110 -259 126 -102 38 -124 45 -175
                                    60 -141 40 -206 48 -394 52 -105 2 -227 0 -271 -4z m462 -320 c120 -25 169
                                    -41 318 -102 44 -18 87 -37 95 -43 8 -6 31 -20 50 -30 258 -140 501 -399 629
                                    -671 18 -38 39 -81 46 -95 7 -14 16 -40 20 -57 3 -18 11 -35 16 -38 5 -4 9
                                    -16 9 -28 0 -12 6 -42 14 -67 70 -217 56 -581 -30 -788 -8 -18 -14 -39 -14
                                    -47 0 -7 -7 -26 -14 -42 -8 -15 -31 -62 -50 -104 -20 -41 -47 -92 -61 -113
                                    -14 -20 -25 -39 -25 -42 0 -3 -26 -40 -59 -82 -68 -89 -192 -214 -271 -273
                                    -72 -53 -146 -132 -156 -167 -4 -15 -10 -178 -13 -362 l-6 -335 -139 -3 c-93
                                    -2 -143 1 -152 9 -12 10 -14 144 -14 828 0 449 4 821 8 826 4 6 38 26 76 45
                                    92 46 165 118 207 206 l34 69 3 170 2 170 -32 38 c-57 66 -133 77 -198 29 -45
                                    -33 -54 -67 -60 -217 -3 -77 -9 -146 -14 -153 -19 -32 -90 -72 -125 -72 -48 0
                                    -109 37 -142 84 -25 38 -26 45 -31 216 -6 193 -12 218 -71 272 -28 25 -39 28
                                    -97 28 -74 -1 -103 -19 -139 -86 -19 -35 -21 -56 -21 -202 0 -140 -3 -168 -19
                                    -200 -32 -61 -102 -112 -155 -112 -40 0 -90 29 -116 68 -23 32 -25 48 -30 174
                                    -6 154 -16 177 -81 210 -87 44 -188 0 -205 -89 -4 -19 -4 -100 -2 -181 5 -144
                                    5 -148 39 -212 47 -89 130 -169 215 -210 38 -17 71 -34 73 -36 6 -5 1 -1656
                                    -5 -1666 -3 -4 -72 -8 -154 -8 -126 0 -149 2 -154 16 -3 9 -6 166 -6 350 0
                                    391 6 365 -121 469 -116 94 -236 213 -300 297 -33 42 -59 81 -59 86 0 5 -6 14
                                    -13 20 -21 17 -123 232 -149 312 -55 170 -78 302 -78 455 0 183 45 416 105
                                    550 33 73 88 182 106 210 10 17 22 37 26 45 4 8 39 58 79 110 81 108 198 226
                                    294 298 57 42 88 63 185 119 94 54 283 124 420 153 117 26 389 26 512 1z
                                    m-174 -1716 l67 -64 -2 -853 -3 -853 -132 -3 -132 -3 -6 43 c-3 24 -5 410 -3
                                    858 l3 816 65 62 c36 34 67 61 70 62 3 0 36 -29 73 -65z"/>
                                <path
                                    d="M3832 5864 c-141 -77 -328 -230 -377 -307 -17 -27 -17 -28 9 -67 41
                                    -62 65 -56 156 37 81 82 188 167 260 205 88 47 100 58 100 93 0 24 -7 39 -26
                                    54 -35 28 -48 26 -122 -15z"/>
                                <path
                                    d="M3249 5223 c-41 -49 -98 -215 -121 -348 -17 -103 -17 -302 1 -385 19
                                    -93 49 -191 67 -217 18 -28 84 -32 108 -7 19 18 21 71 5 111 -27 68 -49 210
                                    -49 312 0 149 29 277 94 425 27 60 26 66 -7 104 -25 27 -78 30 -98 5z"/>
                                <path
                                    d="M2052 5731 c-56 -22 -98 -62 -122 -117 -25 -54 -25 -103 -1 -157 38
                                    -82 108 -127 198 -127 74 0 138 38 177 103 62 107 3 252 -122 301 -53 20 -75
                                    20 -130 -3z"/>
                                <path
                                    d="M6820 5734 c-52 -18 -83 -44 -116 -96 -19 -30 -24 -51 -24 -106 0
                                    -61 4 -74 30 -110 49 -68 94 -92 171 -92 58 0 71 4 110 32 24 17 50 39 57 47
                                    121 151 -46 389 -228 325z"/>
                                <path
                                    d="M1247 4773 c-34 -12 -59 -29 -83 -59 -32 -39 -35 -48 -35 -108 0 -78
                                    17 -114 75 -154 l39 -27 523 0 522 0 44 30 c126 89 96 269 -55 319 -71 24
                                    -958 23 -1030 -1z"/>
                                <path
                                    d="M6705 4772 c-36 -13 -59 -30 -82 -61 -30 -38 -33 -49 -33 -111 l0
                                    -69 50 -50 c46 -46 54 -50 112 -56 35 -3 275 -5 534 -3 l470 3 42 28 c50 33
                                    82 91 82 149 0 59 -46 130 -102 160 -43 23 -44 23 -533 25 -449 2 -494 1 -540
                                    -15z"/>
                                <path
                                    d="M2020 3868 c-95 -61 -133 -169 -91 -261 76 -166 298 -169 382 -5 46
                                    89 5 206 -92 266 -51 31 -150 31 -199 0z"/>
                                <path
                                    d="M6785 3873 c-71 -37 -105 -99 -105 -188 0 -52 5 -73 24 -103 33 -53
                                    56 -70 120 -92 55 -20 57 -20 110 -1 91 32 127 67 148 145 10 37 10 56 0 94
                                    -15 58 -36 87 -91 130 -38 29 -49 32 -109 32 -41 -1 -79 -7 -97 -17z"/>
                                <path
                                    d="M2748 3242 c-40 -19 -727 -688 -758 -737 -18 -29 -24 -69 -21 -131 2
                                    -50 70 -119 136 -140 48 -15 60 -16 97 -4 24 8 54 22 68 33 14 10 181 173 371
                                    362 323 321 346 346 356 390 23 101 -31 197 -129 229 -62 20 -74 20 -120 -2z"/>
                                <path
                                    d="M6123 3242 c-80 -33 -123 -95 -123 -177 0 -79 21 -107 252 -333 122
                                    -120 284 -278 360 -353 121 -119 144 -137 187 -148 155 -40 292 121 220 259
                                    -12 23 -123 138 -272 282 -138 134 -298 289 -356 346 -135 132 -188 157 -268
                                    124z"/>
                                <path
                                    d="M4043 2100 l-383 -5 -37 -38 c-36 -35 -38 -41 -38 -100 0 -75 27
                                    -118 92 -148 40 -18 75 -19 823 -19 763 0 782 0 826 20 83 38 116 128 75 208
                                    -38 74 -42 76 -244 83 -197 8 -514 8 -1114 -1z"/>
                                <path
                                    d="M3835 1555 c-43 -19 -62 -43 -76 -98 -14 -51 -3 -103 30 -141 29 -34
                                    98 -56 172 -56 76 0 71 3 139 -87 44 -57 122 -119 185 -146 44 -18 179 -47
                                    221 -47 51 0 170 32 234 63 64 32 134 92 178 154 12 18 22 36 22 42 0 15 28
                                    21 100 21 128 0 200 55 200 155 0 37 -6 60 -25 88 -47 69 -26 67 -724 66 -496
                                    0 -632 -3 -656 -14z"/>
                            </g>
                        </svg>

                    </button>
                    <button class="card4" onclick="sendEffectToAPI('RUNNING_LIGHTS')">
                        <?xml version="1.0" standalone="no"?>
                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 20010904//EN" "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
                        <svg
                            version="1.0"
                            xmlns="http://www.w3.org/2000/svg"
                            width="900.000000pt"
                            height="900.000000pt"
                            viewbox="0 0 900.000000 900.000000"
                            preserveaspectratio="xMidYMid meet">
                            <g
                                transform="translate(0.000000,900.000000) scale(0.100000,-0.100000)"
                                fill="#000000"
                                stroke="none">
                                <path
                                    d="M2436 5810 c-54 -17 -96 -78 -96 -140 0 -36 23 -91 47 -111 10 -9 37
                                    -23 61 -33 41 -15 45 -15 91 5 26 11 58 34 71 51 47 63 33 163 -29 207 -32 23
                                    -105 33 -145 21z"/>
                                <path
                                    d="M3200 5807 c-74 -26 -114 -140 -73 -205 23 -36 40 -49 90 -73 40 -18
                                    44 -19 85 -3 24 9 58 31 76 50 29 30 32 40 32 94 0 57 -3 64 -37 100 -32 33
                                    -46 40 -88 45 -28 3 -65 0 -85 -8z"/>
                                <path
                                    d="M3979 5813 c-14 -2 -41 -20 -60 -39 -31 -32 -34 -40 -34 -99 0 -73
                                    20 -109 76 -137 90 -45 190 2 220 103 10 33 9 43 -12 85 -13 27 -35 56 -49 66
                                    -26 19 -96 29 -141 21z"/>
                                <path
                                    d="M4745 5812 c-62 -22 -105 -82 -105 -149 0 -103 123 -175 219 -127 54
                                    27 81 73 81 136 0 50 -4 59 -37 97 -33 35 -45 41 -88 45 -27 3 -59 2 -70 -2z"/>
                                <path
                                    d="M5511 5810 c-18 -4 -47 -22 -64 -39 -29 -28 -31 -36 -31 -100 0 -61
                                    3 -74 24 -97 47 -49 120 -67 177 -42 106 46 124 185 33 256 -30 23 -92 33
                                    -139 22z"/>
                                <path
                                    d="M6293 5809 c-17 -5 -45 -27 -62 -48 -26 -32 -31 -48 -31 -91 0 -143
                                    178 -203 273 -91 23 27 27 42 27 91 0 48 -4 64 -26 89 -14 18 -34 35 -43 40
                                    -27 15 -104 21 -138 10z"/>
                                <path
                                    d="M1815 5788 c-50 -29 -73 -87 -61 -153 8 -40 72 -95 112 -95 66 0 134
                                    67 134 132 0 40 -34 94 -70 113 -34 18 -87 19 -115 3z"/>
                                <path
                                    d="M7060 5337 c-61 -24 -119 -77 -150 -136 -29 -57 -28 -187 1 -248 42
                                    -87 158 -160 256 -162 111 -2 241 92 276 201 21 68 21 101 0 164 -32 95 -82
                                    149 -169 183 -70 27 -143 26 -214 -2z"/>
                                <path
                                    d="M2067 5189 c-29 -22 -67 -87 -67 -115 0 -53 84 -134 139 -134 31 0
                                    97 36 115 62 24 33 21 114 -5 146 -50 64 -128 81 -182 41z"/>
                                <path
                                    d="M2613 5181 c-61 -44 -75 -132 -30 -189 17 -22 84 -52 114 -52 59 0
                                    133 74 133 134 0 43 -31 93 -70 115 -53 30 -99 27 -147 -8z"/>
                                <path
                                    d="M3149 5171 c-37 -36 -39 -42 -39 -99 0 -57 2 -61 39 -95 27 -25 53
                                    -37 83 -42 24 -3 765 -5 1648 -3 1528 3 1607 4 1633 21 85 56 88 162 7 231
                                    l-32 26 -1650 0 -1650 0 -39 -39z"/>
                                <path
                                    d="M1519 5152 c-26 -23 -32 -36 -32 -70 0 -47 15 -70 57 -87 59 -25 125
                                    20 126 86 0 30 -7 43 -35 68 -44 38 -75 39 -116 3z"/>
                                <path
                                    d="M3207 4613 c-15 -2 -43 -20 -62 -39 -33 -32 -35 -39 -35 -98 0 -62 1
                                    -65 40 -100 l40 -36 1643 0 1642 0 38 25 c86 59 88 158 4 225 l-31 25 -1626 1
                                    c-894 1 -1638 -1 -1653 -3z"/>
                                <path
                                    d="M2067 4591 c-31 -20 -67 -80 -67 -111 0 -36 38 -97 73 -119 44 -27
                                    90 -27 133 0 50 31 64 56 64 118 0 50 -3 57 -40 93 -36 33 -47 38 -88 38 -29
                                    0 -58 -7 -75 -19z"/>
                                <path
                                    d="M2609 4581 c-47 -39 -65 -97 -45 -143 43 -95 123 -124 199 -73 87 59
                                    88 155 4 219 -49 37 -111 36 -158 -3z"/>
                                <path
                                    d="M1520 4544 c-28 -24 -31 -32 -28 -72 3 -40 8 -50 37 -69 43 -29 59
                                    -29 102 0 29 19 34 29 37 69 3 40 0 48 -28 72 -18 15 -44 26 -60 26 -16 0 -42
                                    -11 -60 -26z"/>
                                <path
                                    d="M7105 4161 c-79 -14 -156 -74 -195 -151 -29 -58 -28 -194 2 -247 50
                                    -88 116 -137 204 -150 155 -23 276 47 326 189 22 64 23 105 2 160 -34 92 -74
                                    141 -145 178 -41 21 -136 32 -194 21z"/>
                                <path
                                    d="M3193 4014 c-51 -22 -83 -72 -83 -130 1 -58 24 -91 87 -123 l42 -21
                                    1606 0 c1581 0 1606 0 1643 20 53 28 66 40 85 82 16 34 16 39 1 80 -21 55 -43
                                    78 -91 94 -58 21 -3243 19 -3290 -2z"/>
                                <path
                                    d="M2054 3983 c-36 -34 -58 -84 -50 -116 12 -47 50 -90 94 -105 39 -13
                                    48 -12 88 3 58 22 84 60 84 120 0 42 -5 52 -39 86 -35 36 -43 39 -93 39 -45 0
                                    -59 -5 -84 -27z"/>
                                <path
                                    d="M2609 3983 c-45 -38 -61 -81 -47 -126 18 -58 58 -90 120 -94 63 -5
                                    94 9 125 56 38 56 25 125 -33 170 -41 32 -123 29 -165 -6z"/>
                                <path
                                    d="M1523 3950 c-33 -19 -39 -36 -34 -90 3 -32 19 -50 55 -65 59 -25 125
                                    20 126 87 0 30 -6 42 -33 62 -41 31 -71 33 -114 6z"/>
                                <path
                                    d="M6309 3439 c-22 -5 -47 -22 -71 -49 -33 -37 -38 -49 -38 -93 0 -61
                                    27 -106 81 -136 46 -26 89 -27 139 -1 55 28 82 71 82 130 0 107 -83 171 -193
                                    149z"/>
                                <path
                                    d="M2398 3416 c-88 -78 -74 -206 29 -259 42 -22 85 -21 128 2 60 32 80
                                    66 80 139 0 64 0 65 -45 103 -42 36 -50 39 -105 39 -48 -1 -65 -5 -87 -24z"/>
                                <path
                                    d="M3177 3420 c-92 -72 -87 -189 11 -255 47 -32 100 -33 150 -1 55 34
                                    73 68 73 132 0 58 -18 93 -69 126 -39 26 -131 24 -165 -2z"/>
                                <path
                                    d="M3950 3423 c-41 -28 -59 -56 -66 -100 -12 -78 15 -133 83 -167 49
                                    -25 94 -20 141 14 94 67 97 177 9 244 -42 32 -128 36 -167 9z"/>
                                <path
                                    d="M4708 3419 c-62 -46 -85 -127 -53 -190 23 -44 91 -89 134 -89 44 0
                                    105 35 131 76 40 60 14 170 -48 207 -42 24 -129 22 -164 -4z"/>
                                <path
                                    d="M5480 3423 c-41 -28 -59 -56 -66 -100 -15 -98 39 -172 133 -180 42
                                    -4 54 -1 90 26 38 28 83 93 83 121 0 28 -43 102 -73 125 -42 31 -128 35 -167
                                    8z"/>
                                <path
                                    d="M1804 3390 c-65 -42 -72 -141 -14 -192 25 -23 39 -28 83 -28 55 1 78
                                    13 110 62 30 47 23 92 -23 138 -34 34 -46 40 -82 40 -26 0 -56 -8 -74 -20z"/>
                            </g>
                        </svg>

                    </button>
                </div>
            </div>
            <button class="delete-button" onclick="sendEffectToAPI('NO_EFFECT')">
                <svg viewbox="0 0 448 512" class="delete-svgIcon">
                    <path
                        d="M135.2 17.7L128 32H32C14.3 32 0 46.3 0 64S14.3 96 32 96H416c17.7 0 32-14.3 32-32s-14.3-32-32-32H320l-7.2-14.3C307.4 6.8 296.3 0 284.2 0H163.8c-12.1 0-23.2 6.8-28.6 17.7zM416 128H32L53.2 467c1.6 25.3 22.6 45 47.9 45H346.9c25.3 0 46.3-19.7 47.9-45L416 128z"></path>
                </svg>
                <span>Clear effect</span>
            </button>
            <label class="slider">
                <p id="percentage" class="percentage">100%</p>
                <input
                    type="range"
                    class="level"
                    min="1"
                    max="255"
                    value="255"
                    onchange="sendBrightnessToAPI(this.value)">
                <svg
                    width="24"
                    height="24"
                    viewbox="0 0 24 24"
                    fill="none"
                    xmlns="http://www.w3.org/2000/svg"
                    class="volume">
                    <path
                        fill-rule="evenodd"
                        clip-rule="evenodd"
                        d="M12 16C14.2091 16 16 14.2091 16 12C16 9.79086 14.2091 8 12 8C9.79086 8 8 9.79086 8 12C8 14.2091 9.79086 16 12 16ZM12 18C15.3137 18 18 15.3137 18 12C18 8.68629 15.3137 6 12 6C8.68629 6 6 8.68629 6 12C6 15.3137 8.68629 18 12 18Z"
                        fill="currentColor"/>
                    <path
                        fill-rule="evenodd"
                        clip-rule="evenodd"
                        d="M11 0H13V4.06189C12.6724 4.02104 12.3387 4 12 4C11.6613 4 11.3276 4.02104 11 4.06189V0ZM7.0943 5.68018L4.22173 2.80761L2.80752 4.22183L5.6801 7.09441C6.09071 6.56618 6.56608 6.0908 7.0943 5.68018ZM4.06189 11H0V13H4.06189C4.02104 12.6724 4 12.3387 4 12C4 11.6613 4.02104 11.3276 4.06189 11ZM5.6801 16.9056L2.80751 19.7782L4.22173 21.1924L7.0943 18.3198C6.56608 17.9092 6.09071 17.4338 5.6801 16.9056ZM11 19.9381V24H13V19.9381C12.6724 19.979 12.3387 20 12 20C11.6613 20 11.3276 19.979 11 19.9381ZM16.9056 18.3199L19.7781 21.1924L21.1923 19.7782L18.3198 16.9057C17.9092 17.4339 17.4338 17.9093 16.9056 18.3199ZM19.9381 13H24V11H19.9381C19.979 11.3276 20 11.6613 20 12C20 12.3387 19.979 12.6724 19.9381 13ZM18.3198 7.0943L21.1923 4.22183L19.7781 2.80762L16.9056 5.6801C17.4338 6.09071 17.9092 6.56608 18.3198 7.0943Z"
                        fill="currentColor"/>
                </svg>
            </label>
        </div>
        <div class="overlay" id="overlay"></div>
        <div class="settings-card" id="settingsCard">
            <span class="title">Reset Settings</span>
            <p class="description">If you reset the Settings of Sonicsync you have to enter
                the WLan-Credentials again.</p>
            <div class="actions">
                <button class="cancel" onclick="cancelAction()">
                    Cancel
                </button>
                <button class="accept" onclick="acceptAction()">
                    Accept
                </button>
            </div>
        </div>

        <script>
            const colors = [
                {
                    r: 0xe4,
                    g: 0x3f,
                    b: 0x00
                }, {
                    r: 0xfa,
                    g: 0xe4,
                    b: 0x10
                }, {
                    r: 0x55,
                    g: 0xcc,
                    b: 0x3b
                }, {
                    r: 0x09,
                    g: 0xad,
                    b: 0xff
                }, {
                    r: 0x6b,
                    g: 0x0e,
                    b: 0xfd
                }, {
                    r: 0xe7,
                    g: 0x0d,
                    b: 0x86
                }, {
                    r: 0xe4,
                    g: 0x3f,
                    b: 0x00
                }
            ];

            document.addEventListener('DOMContentLoaded', function () {
                const colorWheel = document.getElementById('color-wheel');
                const crosshair = document.getElementById('crosshair');

                colorWheel.addEventListener('click', function (e) {
                    var rect = colorWheel.getBoundingClientRect();
                    var x = e.clientX - rect.left;
                    var y = e.clientY - rect.top;

                    crosshair.style.left = x + 'px';
                    crosshair.style.top = y + 'px';
                    var rect = e
                        .target
                        .getBoundingClientRect();
                    var x = 2 * (e.clientX - rect.left) / (rect.right - rect.left) - 1;
                    var y = 1 - 2 * (e.clientY - rect.top) / (rect.bottom - rect.top);
                    var a = ((Math.PI / 2 - Math.atan2(y, x)) / Math.PI * 180);
                    if (a < 0) 
                        a += 360;
                    a = a / 360 * (colors.length - 1);
                    var a0 = Math.floor(a) % colors.length;
                    var a1 = (a0 + 1) % colors.length;
                    var c0 = colors[a0];
                    var c1 = colors[a1];
                    var a1w = a - Math.floor(a);
                    var a0w = 1 - a1w;
                    var color = {
                        r: c0.r * a0w + c1.r * a1w,
                        g: c0.g * a0w + c1.g * a1w,
                        b: c0.b * a0w + c1.b * a1w
                    };
                    var r = Math.sqrt(x * x + y * y);
                    if (r > 1) 
                        r = 1;
                    var cw = r < 0.8
                        ? (r / 0.8)
                        : 1;
                    var ww = 1 - cw;
                    color.r = Math.round(color.r * cw + 255 * ww);
                    color.g = Math.round(color.g * cw + 255 * ww);
                    color.b = Math.round(color.b * cw + 255 * ww);
                    pixelColor = "rgb(" + color.r + ", " + color.g + ", " + color.b + ")";
                    sendColorToAPI(pixelColor);
                });
            });

            function sendBrightnessToAPI(brightnessValue) {
                var brightnessPercentage = Math.max(
                    Math.round((brightnessValue / 255) * 100),
                    1
                );
                var percentageElement = document.getElementById("percentage");

                percentageElement.textContent = `${brightnessPercentage}%`;

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

            function sendEffectToAPI(effect) {
                var formData = new FormData();
                formData.append('effect', effect);

                fetch('/setEffect', {
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

            function sendTurnLedOnOff() {
                fetch('/on_off', {method: 'POST'})
                    .then(response => response.text())
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
                    .then(response => response.text())
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            const overlay = document.getElementById('overlay');
            const settingsCard = document.getElementById('settingsCard');

            function showSettingsCard() {
                settingsCard.style.display = 'block';
                overlay.style.display = 'block';
                document
                    .body
                    .classList
                    .add('overlay-active');
            }

            function acceptAction() {
                fetch('/reset', {method: 'POST'})
                    .then(response => response.text())
                    .then(data => {
                        console.log('API response:', data);
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }

            function cancelAction() {
                hideOverlay();
            }

            function hideOverlay() {
                settingsCard.style.display = 'none';
                overlay.style.display = 'none';
                document
                    .body
                    .classList
                    .remove('overlay-active');
            }
        </script>
    </body>

</html>
)HTML";

#endif