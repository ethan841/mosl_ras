let express = require('express');
let {spawn} = require('child_process');
let app = express();

app.listen(4500, () => {
	console.log('DCAP Attestation Server is running');
});



app.get('/dcap_att_start', (req, res) => {
	console.log('host client connected');

	let result;
	const dcap_att = spawn('/home/mobileosdcaps/SGX/mosl/SGXDataCenterAttestationPrimitives/SampleCode/QuoteVerificationSample/app', []);
	dcap_att.stdout.on('data', (data) => {
		result = data.toString();
	});
	dcap_att.on('close', (code) => {
		console.log(result);
		console.log("waiting for another request");
		res.send('attestation complete');
	});
});

