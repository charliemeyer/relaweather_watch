//get the user's location, locationSuccess talks to relaweather
function get_weather() {
        navigator.geolocation.getCurrentPosition(
                locationSuccess,
                locationError,
                {timeout: 15000, maximumAge: 60000}
        );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
        function(e) {
                console.log("PebbleKit JS ready!");
                get_weather();  
        }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
        function(e) {
                console.log("AppMessage received!");
                get_weather();
        }
);

//get the weather from RelaWeather API
var xhrRequest = function (url, type, callback) {
                var xhr = new XMLHttpRequest();
                xhr.onload = function () {
                callback(this.responseText);
        };
        xhr.open(type, url);
        xhr.send();
};

//get the weather when you've got the user's location
function locationSuccess(pos) {
        var url = "http://relaweather.appspot.com/api?lat=" + pos.coords.latitude + "&lng=" + pos.coords.longitude;
        xhrRequest(url, "GET", function(responseText) {
                var data = JSON.parse(responseText);
                var forecast = data.current;
                var location = data.city + ", " + data.state;
                console.log("Conditions are " + forecast);
                var dictionary = {
                        "KEY_LOCATION": location,
                        "KEY_FORECAST": forecast
                };
                Pebble.sendAppMessage(dictionary,
                        function(e) {
                                console.log("Weather info sent to Pebble successfully!");
                        },
                        function(e) {
                                console.log("Error sending weather info to Pebble!");
                        }
                );
        });
        
}

function locationError(err) {
        console.log("Error requesting location!");
}
