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
	//var xhrHash= new XMLHttpRequest();
	var xhrDiff= new XMLHttpRequest();
	xhrDiff.open('GET','http://dogechain.info/chain/Dogecoin/q/getdifficulty?cache='+(Math.random()*1000000),true);
	var xhrBlock= new XMLHttpRequest();
	xhrBlock.open('GET','http://dogechain.info/chain/Dogecoin/q/getblockcount?cache='+(Math.random()*1000000),true);
	var xhrBTC= new XMLHttpRequest();
	xhrBTC.open('GET','https://api.prelude.io/last/DOGE',true);
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'https://api.prelude.io/last-usd/DOGE', true);


	//timeouts?
	xhr.timeout = timeoutForAPIRequest;
	xhrBTC.timeout = timeoutForAPIRequest;

	xhr.onload = function(e) {
		if (xhr.readyState == 4) {
			if (xhr.status == 200) {


				block=xhrBlock.responseText+'';
				//blockNum=(parseFloat(xhrBlock.responseText)-2)+'';


				//xhrHash.open('GET','http://dogechain.info/chain/Dogecoin/q/nethash/1/'+blockNum+'?format=json',true);




				//Parsing the JSON Sources
				res = JSON.parse(xhr.responseText);
				resBTC = JSON.parse(xhrBTC.responseText);


				//doing the required Math and stuff
				pricePerK=(parseFloat(res.last)*1000)+'';
				pricePerBTC=(parseFloat(resBTC.last)*100000000)+'';
				difficulty=xhrDiff.responseText+'';

				//resHash = JSON.parse(xhrHash.responseText);
				//hashrate=(parseFloat(resHash[8])/1000000000)+" GH/s";
				//Updating the Pebble
        
        console.log("I am running now!");
				sendAppMessage({
					'price_usdk': pricePerK,
					'price_doge': pricePerBTC,
					'net_block': block,
				//	'net_hash': hashrate,
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
//xhrHash.send(null);

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
