let map;
var mrkr = null;
// temporary latitude and longitudes
var lat = null;
var lng =  null;
var myLocation = null; 

function getPosition(){
    var gpsData = null;
    $.get("gps_data.txt", function(data){
      gpsData = data;
      console.log(gpsData);
      vals = gpsData.split(",");
      lat = parseFloat(vals[0]);
      lng = parseFloat(vals[1]);

      if(!isNaN(lat) || !isNaN(lng)){
          changeMarkerPosition();
      }else{ return; }
    },"text");
     
    
}

function changeMarkerPosition() {
 
    myLocation = new google.maps.LatLng(lat, lng);
    console.log(myLocation);
    mrkr.setPosition(myLocation);
    map.setCenter(myLocation);
}

function initMap() {
  myLocation = new google.maps.LatLng(lat, lng);
  map = new google.maps.Map(document.getElementById("map"), {
    center: myLocation,
    zoom: 19,
  });

  mrkr = new google.maps.Marker({
    position: myLocation,
    map,
    title: "My Location",
  });

  setInterval(getPosition,1000);
}
