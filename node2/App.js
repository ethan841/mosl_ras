let express = require('express');
let {spawn} = require('child_process');
let app = express();
let fs = require('fs');

var exec = require('child_process').exec;

app.listen(4000, () => {
	console.log('Attestation Server is running');
});

app.get('/dcap_att_start', (req, res) => {
	console.log('host client connected - DCAP');

	let result;
	const dcap_att = spawn('/home/mobileosdcaps/SGX/mosl/SGXDataCenterAttestationPrimitives/SampleCode/QuoteVerificationSample/app', []);
	console.log('Init DCAP attestation server');
	res.send('init complete');
	dcap_att.stdout.on('data', (data) => {
		result = data.toString();
	});
	dcap_att.on('close', (code) => {
		console.log(result);

		fs.unlink('quote.dat', function(err){
			if(err) throw err;
			console.log('quote.dat deleted');
		});

		console.log("waiting for another attestation request");
	});
	
});


app.get('/epid_att_start', (req, res) => {

	console.log('host client connected - EPID');

	let result2;
	const epid_att = spawn('/home/mobileosdcaps/SGX/mosl/sgx-ra-sample/run-server', []);
	console.log('Init EPID attestation server');
	res.send('init complete');
	epid_att.stdout.on('data', (data) => {
		result2 = data.toString();
	});
	epid_att.on('close', (code) => {
		console.log(result2);
		console.log("waiting for another attestation request");
	});
	/*
	res.send('init complete');
	child = exec("/home/mobileosdcaps/SGX/mosl/sgx-ra-sample/run-server", function (error, result, stderr){
		console.log(result);
	});
	*/
});

