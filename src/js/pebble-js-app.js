//Works covered by CC BY-NC-SA 4.0

var maxTriesForSendingAppMessage = 3;
var timeoutForAppMessageRetry = 3000;
var timeoutForAPIRequest = 12000;

function sendAppMessage(message, numTries, transactionId) {
	numTries = numTries || 0;
	if (numTries < maxTriesForSendingAppMessage) {
		numTries++;
		console.log('Sending AppMessage to Pebble: ' + JSON.stringify(message));
		Pebble.sendAppMessage(
			message, function() {}, function(e) {
				console.log('Failed sending AppMessage for transactionId:' + e.data.transactionId + '. Error: ' + e.data.error.message);
				setTimeout(function() {
					sendAppMessage(message, numTries, e.data.transactionId);
				}, timeoutForAppMessageRetry);
			}
		);
	} else {
		console.log('Failed sending AppMessage for transactionId:' + transactionId + '. Bailing. ' + JSON.stringify(message));
	}
}

function makeRequest() {
	var pricePerK;
	var pricePerBTC;
	var hashrate;
	var block;
	var difficulty;



	//Creating and Opening the needed XMLHttpRequest Objects
	var xhrHash= new XMLHttpRequest();
  xhrHash.open('GET','http://narwhy.pw/shibe/nethash/',true);
	var xhrDiff= new XMLHttpRequest();
	xhrDiff.open('GET','http://dogechain.info/chain/Dogecoin/q/getdifficulty?cache='+(Math.random()*1000000),true);
	var xhrBlock= new XMLHttpRequest();
	xhrBlock.open('GET','http://dogechain.info/chain/Dogecoin/q/getblockcount?cache='+(Math.random()*1000000),true);
	var xhrBTC= new XMLHttpRequest();
	xhrBTC.open('GET','https://api.prelude.io/last/DOGE',false);
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'https://api.prelude.io/last-usd/DOGE', true);


	//timeouts?
	xhr.timeout = timeoutForAPIRequest;
	xhrBTC.timeout = timeoutForAPIRequest;

	xhr.onload = function(e) {
		if (xhr.readyState == 4) {
			if (xhr.status == 200) {


				block=xhrBlock.responseText;
        block=block+'';
				
        hashrate=xhrHash.responseText;
        console.log(hashrate+'');
        

				//Parsing the JSON Sources
				var res = JSON.parse(xhr.responseText);
				var resBTC = JSON.parse(xhrBTC.responseText);


				//doing the required Math and stuff
				pricePerK=(parseFloat(res.last)*1000);
				pricePerBTC=(parseFloat(resBTC.last)*100000000);
				difficulty=xhrDiff.responseText;
        
				hashrate=(parseFloat(hashrate)/1000000000);
				difficulty=parseFloat(difficulty);
        
        //Doing some Formatting so everything fits in its space.
        pricePerK=pricePerK.toFixed(2);
        pricePerBTC=pricePerBTC.toFixed(0);
        difficulty=difficulty.toFixed(1);
        hashrate=hashrate.toFixed(0);
        hashrate=hashrate+"GH/s";
        //Updating the Pebble
        
        console.log("I am running now!");
        console.log(hashrate);
				sendAppMessage({
					'price_usdk': pricePerK,
					'price_doge': pricePerBTC,
					'net_block': block,
					'net_hash': hashrate,
					'net_diff': difficulty
				});

			} else {
				console.log('Request returned error code ' + xhr.status.toString());
				sendAppMessage({'item_name': 'Error: ' + xhr.statusText});
			}
		}
  }
	xhr.ontimeout = function() {
		console.log('Error: request timed out!');
		sendAppMessage({'item_name': 'Error: Request timed out!'});
	};
	xhr.onerror = function(e) {
		console.log(JSON.stringify(e));
		sendAppMessage({'item_name': 'Error: Failed to connect!'});
	};
xhr.send(null);
xhrBTC.send(null);
xhrBlock.send(null);
xhrDiff.send(null);
xhrHash.send(null);

}

Pebble.addEventListener('ready', function(e) {});

Pebble.addEventListener('appmessage', function(e) {
	console.log('AppMessage received from Pebble: ' + JSON.stringify(e.payload));

	makeRequest();
});

Pebble.addEventListener("ready", function(e) {
	console.log("PEBBLEJS connected!" + e.ready);
	Pebble.sendAppMessage({"message": "ready"});
});
