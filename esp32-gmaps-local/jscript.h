const String javascript PROGMEM = "let map;"
"var mrkr = null;"
"var lat = null;"
"var lng = null;"
"var myLocation = null;"

"function getPosition(){"
    "xmlhttp=new XMLHttpRequest();"
    "xmlhttp.open('GET','/loc',false);"
    "xmlhttp.send();"

    "var data = xmlhttp.responseText;"
    "console.log(data);"
    "vals = data.split(',');"
    "lat = parseFloat(vals[0]);"
    "lng = parseFloat(vals[1]);"
    
    "changeMarkerPosition();"
"}"

"function changeMarkerPosition() {"
 
    "myLocation = new google.maps.LatLng(lat, lng);"
    "console.log(myLocation);"
    "mrkr.setPosition(myLocation);"
    "map.setCenter(myLocation);"
"}"

"function initMap() {"
  "map = new google.maps.Map(document.getElementById('map'), {"
    "center: myLocation,"
    "zoom: 19,"
  "});"

  "mrkr = new google.maps.Marker({"
    "position: myLocation,"
    "map,"
    "title: 'My Location',"
  "});"

  "setInterval(getPosition,1000);"
"}";
