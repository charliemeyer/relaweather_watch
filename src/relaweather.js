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
  }                     
);

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
        var url = "http://relaweather.appspot.com/api?lat=" + pos.coords.latitude + "&lng=" + pos.coords.longitude;
        //$.get('/api?lat='+myLat+'&lng='+myLng, get_success);
        xhrRequest(url, "GET", function(responseText) {
                var data = JSON.parse(responseText);
                var conditions = data.current;    
                console.log("Conditions are " + conditions);
        });
        
        //$.get('/api?lat='+myLat+'&lng='+myLng, get_success);
}

function locationError(err) {
  console.log("Error requesting location!");
}


