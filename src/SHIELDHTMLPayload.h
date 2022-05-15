#ifndef SHIELDWEBINTERFACE_h
#define SHIELDWEBINTERFACE_h

const char LOGIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
<head>
	<title>SHIELD | Login</title>
</head>

<style>
	body {font-family: Arial, Helvetica, sans-serif;}
	* {box-sizing: border-box;}

	/* Full-width input fields */
	input[type=text], input[type=password] {
		width: 100%;
		padding: 12px 20px;
        border: 1px solid #ccc;
		border-radius: 10px;
		margin: 8px 0;
        opacity: 0.85;
		display: inline-block;
		font-size: 15px;
        line-height: 20px;
        text-decoration: none; /* remove underline from anchors */
		box-sizing: border-box;
	}

	/* Add a background color when the inputs get focus */
	input[type=text]:focus, input[type=password]:focus {
		background-color: #ddd;
		outline: none;
        border-color: #AF52F2;
	}

	/* Set a style for all buttons */
	button {
		background-color: #422057FF;
		color: white;
		padding: 14px 20px;
		margin-top: 12px;
		margin-bottom: 15px;        
		border: none;
		border-radius: 10px;
		cursor: pointer;
		width: 100%;
		font-size: 17px;
	}

	/* Mouse Hover */
	button:hover {
		opacity: 0.8;
	}

	/* Float cancel and signup buttons and add an equal width */
	.signupbtn {
		float: left;
		width: 100%;
	}

	/* Add padding to container elements */
	.container {
		padding: 16px;
	}

	/* The Modal (background) */
	.modal {
		display: none; /* Hidden by default */
		position: fixed; /* Stay in place */
		z-index: 1; /* Sit on top */
		left: 0;
		top: 0;
		width: 100%; /* Full width */
		height: 100%; /* Full height */
		overflow: auto; /* Enable scroll if needed */
		background-color: #474e5d;
		padding-top: 50px;
	}

	/* Modal Content/Box */
	.modal-content {
		background-color: #fefefe;
		margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
		border: 1px solid #888;
		border-radius: 10px;
		width: 80%; /* Could be more or less, depending on screen size */
	}


	/* Zoom Animation */
	.animate {
		-webkit-animation: animatezoom 0.6s;
		animation: animatezoom 0.6s
	}

	@-webkit-keyframes animatezoom {
		from {-webkit-transform: scale(0)} 
		to {-webkit-transform: scale(1)}
	}

	@keyframes animatezoom {
		from {transform: scale(0)} 
		to {transform: scale(1)}
	}

	/* Style the horizontal ruler */
	hr {
		border: 1px solid #f1f1f1;
		margin-top: 0px;
		margin-bottom: 10px;
	}

	/* Clear floats */
	.clearfix::after {
		content: "";
		clear: both;
		display: table;
	}

	/* Change styles for cancel button and signup button on extra small screens */
	@media screen and (max-width: 300px) {
		.signupbtn {
			width: 100%;
		}
	}

	/* Header */
	h1 {
		margin-top: 0px;
		text-align: center;
	}
    
	/* Text Block */
    a {
    	display: block;
        text-align: right;
        padding-top: 10px;
        margin: auto;
        font-color: #9112EC;
	}

</style>
<body onload="loaded();">
	<div id="activate-page" class="modal">
		<form class="modal-content animate" action="/main.php" method="post">
			<div class="container">
				<h1>SHIELD | Login</h1>
                <p>Login to SHIELD Web Interface.</p>
				<hr>
				<input type="text" placeholder="Username" name="username" required>
				<input type="password" placeholder="Password" name="password" required>
				<button type="submit" class="signupbtn">Login</button>
                <a href="#">Forgot password?</a>
			</div>
		</form>
	</div>

	<script>
		// Loads the Signup modal on page load
		function loaded() {
          document.getElementById('activate-page').style.display='block'
          style="width:auto;"
		}
	</script>
</body>
</html>
)=====";

const char ACTIVATE_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<head>
	<title>SHIELD | Activate</title>
</head>

<style>
	body {font-family: Arial, Helvetica, sans-serif;}
	* {box-sizing: border-box;}

	/* Full-width input fields */
	input[type=text], input[type=password] {
		width: 100%;
		padding: 12px 20px;
        border: 1px solid #ccc;
		border-radius: 10px;
		margin: 8px 0;
        opacity: 0.85;
		display: inline-block;
		font-size: 15px;
        line-height: 20px;
        text-decoration: none; /* remove underline from anchors */
		box-sizing: border-box;
	}

	/* Add a background color when the inputs get focus */
	input[type=text]:focus, input[type=password]:focus {
		background-color: #ddd;
		outline: none;
        border-color: #AF52F2;
	}

	/* Set a style for all buttons */
	button {
		background-color: #422057FF;
		color: white;
		padding: 14px 20px;
		margin-top: 12px;
		margin-bottom: 15px;        
		border: none;
		border-radius: 10px;
		cursor: pointer;
		width: 100%;
		font-size: 17px;
	}

	/* Mouse Hover */
	button:hover {
		opacity: 0.8;
	}

	/* Float cancel and signup buttons and add an equal width */
	.signupbtn {
		float: left;
		width: 100%;
	}

	/* Add padding to container elements */
	.container {
		padding: 16px;
	}

	/* The Modal (background) */
	.modal {
		display: none; /* Hidden by default */
		position: fixed; /* Stay in place */
		z-index: 1; /* Sit on top */
		left: 0;
		top: 0;
		width: 100%; /* Full width */
		height: 100%; /* Full height */
		overflow: auto; /* Enable scroll if needed */
		background-color: #474e5d;
		padding-top: 50px;
	}

	/* Modal Content/Box */
	.modal-content {
		background-color: #fefefe;
		margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
		border: 1px solid #888;
		border-radius: 10px;
		width: 80%; /* Could be more or less, depending on screen size */
	}


	/* Zoom Animation */
	.animate {
		-webkit-animation: animatezoom 0.6s;
		animation: animatezoom 0.6s
	}

	@-webkit-keyframes animatezoom {
		from {-webkit-transform: scale(0)} 
		to {-webkit-transform: scale(1)}
	}

	@keyframes animatezoom {
		from {transform: scale(0)} 
		to {transform: scale(1)}
	}

	/* Style the horizontal ruler */
	hr {
		border: 1px solid #f1f1f1;
		margin-top: 0px;
		margin-bottom: 10px;
	}

	/* Clear floats */
	.clearfix::after {
		content: "";
		clear: both;
		display: table;
	}

	/* Change styles for cancel button and signup button on extra small screens */
	@media screen and (max-width: 300px) {
		.signupbtn {
			width: 100%;
		}
	}

	/* Header */
	h1 {
		margin-top: 0px;
		text-align: center;
	}

</style>
<body onload="loaded();">
	<div id="activate-page" class="modal">
		<form class="modal-content animate" action="/activate.php" method="post">
			<div class="container">
				<h1>SHIELD | Activate</h1>
				<p>To activate SHIELD, please fill in this form.</p>
				<hr>
				<input type="text" placeholder="Username" name="username" required>
				<input type="password" placeholder="Password" name="password" required>
				<input type="password" placeholder="Repeat Password" name="psw_repeat" required>

				<p>By activating the device, you agree to SHIELD's <a href="#" style="color:dodgerblue">Terms & Privacy</a>.</p>

				<div class="clearfix">
					<button type="submit" class="signupbtn">Activate</button>
				</div>
			</div>
		</form>
	</div>

	<script>        
		// Loads the Signup modal on page load
		function loaded() {
			document.getElementById('activate-page').style.display='block'
			style="width:auto;"
		}
	</script>
</body>
</html>
)=====";

const char MAIN_EXPOSED_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
	
    * { box-sizing: border-box; }
    body { font-family: Arial, Helvetica, sans-serif; }

    /* Full-width input fields */
	input[type=text]{
		width: 100%;
		padding: 12px 20px;
        border: 1px solid #ccc;
		border-radius: 10px;
		margin: 8px 0;
        opacity: 0.85;
		display: inline-block;
		font-size: 15px;
        line-height: 20px;
        text-decoration: none; /* remove underline from anchors */
		box-sizing: border-box;
	}

	/* Add a background color when the inputs get focus */
	input[type=text]:focus{
		background-color: #ddd;
		outline: none;
        border-color: #AF52F2;
	}

    /* Float four columns side by side */
    .column {
        float: left;
        width: 50%;
        padding: 0 10px;
    }

    /* Add padding to container elements */
    .container {
        padding: 16px;
    }

    /* Remove extra left and right margins, due to padding */
    .row {margin: 0 -5px;}

    /* Clear floats after the columns */
    .row:after {
        content: "";
        display: table;
        clear: both;
    }

    /* Style the counter cards */
    .card {
        border-radius: 10px;
        box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
        padding: 16px;
        text-align: center;
        background-color: #f1f1f1;
        margin-bottom: 30px;
    }
    
    .EScard {
        border-radius: 10px;
        box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
        padding: 16px;
        text-align: center;
        background-color: #C41E3A;
        margin-bottom: 40px;
        color: White;
    }

    a:link{
        text-decoration: none;
    }

    p {
        text-align: center;
    }
    
    /* Set a style for all buttons */
	button {
		background-color: #422057FF;
		color: white;
		padding: 14px 20px;
		margin-top: 12px;
		margin-bottom: 1px;        
		border: none;
		border-radius: 10px;
		cursor: pointer;
		width: 100%;
		font-size: 17px;
	}

	/* Mouse Hover */
	button:hover {
		opacity: 0.8;
	}

	/* Float cancel and signup buttons and add an equal width */
	.btns {
		float: left;
		width: 100%;
	}

	/* Add padding to container elements */
	.container {
		padding: 16px;
	}

	/* The Modal (background) */
	.modal {
		display: none; /* Hidden by default */
		position: fixed; /* Stay in place */
		z-index: 1; /* Sit on top */
		left: 0;
		top: 0;
		width: 100%; /* Full width */
		height: 100%; /* Full height */
		overflow: auto; /* Enable scroll if needed */
		background-color: #474e5d;
		padding-top: 50px;
	}

	/* Modal Content/Box */
	.modal-content {
		background-color: #fefefe;
		margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
		border: 1px solid #888;
		border-radius: 10px;
		width: 80%; /* Could be more or less, depending on screen size */
	}


	/* Zoom Animation */
	.animate {
		-webkit-animation: animatezoom 0.6s;
		animation: animatezoom 0.6s
	}

	@-webkit-keyframes animatezoom {
		from {-webkit-transform: scale(0)} 
		to {-webkit-transform: scale(1)}
	}

	@keyframes animatezoom {
		from {transform: scale(0)} 
		to {transform: scale(1)}
	}

	/* Style the horizontal ruler */
	hr {
		border: 1px solid #f1f1f1;
		margin-top: 0px;
		margin-bottom: 10px;
	}

	/* Clear floats */
	.clearfix::after {
		content: "";
		clear: both;
		display: table;
	}

	/* Change styles for cancel button and signup button on extra small screens */
	@media screen and (max-width: 300px) {
		.signupbtn {
			width: 100%;
		}
	}

	/* Header */
	h1 {
    	margin-top: 0px;
        margin-bottom: 0px;
		text-align: center;
        font-size: 42px;
	}
    
    h2 {
		text-align: center;
        font-size: 25px;
	}

	/* Extra styles for the cancel button */
	.cancelbtn {
		width: auto;
		padding: 10px 18px;
		background-color: #f44336;
	}

</style>
</head>

<body>
<div class="container">
    <div class="EScard">
        <span> <h2>You are </h2>
            <h1>EXPOSED</h1> 
            <h2> in the last 17 days</h2>
        </span>
    </div>

    <div class="row">
        <div class="column">
            <a href="#" id ="btn">
                <div class="card">     
                <span> <h2>Share Exposure Status</h2>
                </span>
                </div>
            </a>
        </div>

        <div class="column" >
            <a href="/faqs.php">
                <div class="card">
                    <span><h2>Frequently Asked Questions</h2></span>
                </div>
            </a>
        </div>
    </div>

    <button type="submit" class="logout_btn" onclick="location.href='/';">Log out</button>
</div>

	<div id="case_mdl" class="modal">
		<form class="modal-content animate" action="/main.php" method="post">
			<div class="container">
                <h1>SHIELD</h1>
				<p>Share your positive test result.</p>
				<hr>
				<input type="text" placeholder="Case Number" name="case_number" required>
				<p>By sending your exposure history, you agree to <a href="#" style="color:dodgerblue">Terms of Condition</a> and in accordance to Republic Act 11332 (Mandatory Reporting of Notifiable Diseases and Health Events of Public Health Concern Act) and Republic Act 10173 (Data Privacy Act of 2012).</p>

				<div class="clearfix">
					<button type="submit" onclick="document.getElementById('case_number').style.display='none'" class="btns">Share Now</button>
					<button type="button" onclick="document.getElementById('case_mdl').style.display='none'" class="btns">Cancel</button>
				</div>
			</div>
		</form>
	</div>

<script>
    var modal = document.getElementById('case_mdl');
    var positive = document.getElementById("btn");

    positive.onclick = function(){
        modal.style.display = "block";
    }

</script>

</body>
</html>
)=====";

const char MAIN_NOT_EXPOSED_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
	
    * { box-sizing: border-box; }
    body { font-family: Arial, Helvetica, sans-serif; }

    /* Full-width input fields */
	input[type=text]{
		width: 100%;
		padding: 12px 20px;
        border: 1px solid #ccc;
		border-radius: 10px;
		margin: 8px 0;
        opacity: 0.85;
		display: inline-block;
		font-size: 15px;
        line-height: 20px;
        text-decoration: none; /* remove underline from anchors */
		box-sizing: border-box;
	}

	/* Add a background color when the inputs get focus */
	input[type=text]:focus{
		background-color: #ddd;
		outline: none;
        border-color: #AF52F2;
	}

    /* Float four columns side by side */
    .column {
        float: left;
        width: 50%;
        padding: 0 10px;
    }

    /* Add padding to container elements */
    .container {
        padding: 16px;
    }

    /* Remove extra left and right margins, due to padding */
    .row {margin: 0 -5px;}

    /* Clear floats after the columns */
    .row:after {
        content: "";
        display: table;
        clear: both;
    }

    /* Style the counter cards */
    .card {
        border-radius: 10px;
        box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
        padding: 16px;
        text-align: center;
        background-color: #f1f1f1;
        margin-bottom: 30px;
    }
    
    .EScard {
        border-radius: 10px;
        box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
        padding: 16px;
        text-align: center;
        background-color: #097969 ;
        margin-bottom: 40px;
        color: White;
    }

    a:link{
        text-decoration: none;
    }

    p {
        text-align: center;
    }
    
    /* Set a style for all buttons */
	button {
		background-color: #422057FF;
		color: white;
		padding: 14px 20px;
		margin-top: 12px;
		margin-bottom: 1px;        
		border: none;
		border-radius: 10px;
		cursor: pointer;
		width: 100%;
		font-size: 17px;
	}

	/* Mouse Hover */
	button:hover {
		opacity: 0.8;
	}

	/* Float cancel and signup buttons and add an equal width */
	.btns {
		float: left;
		width: 100%;
	}

	/* Add padding to container elements */
	.container {
		padding: 16px;
	}

	/* The Modal (background) */
	.modal {
		display: none; /* Hidden by default */
		position: fixed; /* Stay in place */
		z-index: 1; /* Sit on top */
		left: 0;
		top: 0;
		width: 100%; /* Full width */
		height: 100%; /* Full height */
		overflow: auto; /* Enable scroll if needed */
		background-color: #474e5d;
		padding-top: 50px;
	}

	/* Modal Content/Box */
	.modal-content {
		background-color: #fefefe;
		margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
		border: 1px solid #888;
		border-radius: 10px;
		width: 80%; /* Could be more or less, depending on screen size */
	}


	/* Zoom Animation */
	.animate {
		-webkit-animation: animatezoom 0.6s;
		animation: animatezoom 0.6s
	}

	@-webkit-keyframes animatezoom {
		from {-webkit-transform: scale(0)} 
		to {-webkit-transform: scale(1)}
	}

	@keyframes animatezoom {
		from {transform: scale(0)} 
		to {transform: scale(1)}
	}

	/* Style the horizontal ruler */
	hr {
		border: 1px solid #f1f1f1;
		margin-top: 0px;
		margin-bottom: 10px;
	}

	/* Clear floats */
	.clearfix::after {
		content: "";
		clear: both;
		display: table;
	}

	/* Change styles for cancel button and signup button on extra small screens */
	@media screen and (max-width: 300px) {
		.signupbtn {
			width: 100%;
		}
	}

	/* Header */
	h1 {
    	margin-top: 0px;
        margin-bottom: 0px;
		text-align: center;
        font-size: 42px;
	}
    
    h2 {
		text-align: center;
        font-size: 25px;
	}

	/* Extra styles for the cancel button */
	.cancelbtn {
		width: auto;
		padding: 10px 18px;
		background-color: #f44336;
	}

</style>
</head>

<body>
<div class="container">
    <div class="EScard">
        <span> <h2>You are </h2>
            <h1>NOT EXPOSED</h1> 
            <h2> in the last 17 days</h2>
        </span>
    </div>

    <div class="row">
        <div class="column">
            <a href="#" id ="btn">
                <div class="card">     
                <span> <h2>Share Exposure Status</h2>
                </span>
                </div>
            </a>
        </div>

        <div class="column" >
            <a href="/faqs.php">
                <div class="card">
                    <span><h2>Frequently Asked Questions</h2></span>
                </div>
            </a>
        </div>
    </div>

    <button type="submit" class="logout_btn" onclick="location.href='/';">Log out</button>
</div>

	<div id="case_mdl" class="modal">
		<form class="modal-content animate" action="/main.php" method="post">
			<div class="container">
                <h1>SHIELD</h1>
				<p>Share your positive test result.</p>
				<hr>
				<input type="text" placeholder="Case Number" name="case_number" required>
				<p>By sending your exposure history, you agree to <a href="#" style="color:dodgerblue">Terms of Condition</a> and in accordance to Republic Act 11332 (Mandatory Reporting of Notifiable Diseases and Health Events of Public Health Concern Act) and Republic Act 10173 (Data Privacy Act of 2012).</p>

				<div class="clearfix">
					<button type="submit" onclick="document.getElementById('case_number').style.display='none'" class="btns">Share Now</button>
					<button type="button" onclick="document.getElementById('case_mdl').style.display='none'" class="btns">Cancel</button>
				</div>
			</div>
		</form>
	</div>

<script>
    var modal = document.getElementById('case_mdl');
    var positive = document.getElementById("btn");

    positive.onclick = function(){
        modal.style.display = "block";
    }

</script>

</body>
</html>
)=====";

const char FAQS_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  /* Style the counter cards */
.card {
border-radius: 10px;
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
  padding: 16px;
  text-align: center;
  background-color: #f1f1f1;
  margin-bottom: 30px;
  
}
.collapsible {
  background-color: #422057FF;
  color: white;
  cursor: pointer;
  padding: 18px;
  width: 100%;
  border: none;
  text-align: left;
  outline: none;
  font-size: 15px;
  border-radius: 10px;
  margin-bottom: 10px;
}

button {
		background-color: #422057FF;
		color: white;
		padding: 14px 20px;
		margin-top: 12px;
		margin-bottom: 15px;        
		border: none;
		border-radius: 10px;
		cursor: pointer;
		width: 100%;
		font-size: 17px;
	}

	/* Mouse Hover */
	button:hover {
		opacity: 0.8;
	}

.active, .collapsible:hover {
  background-color: #555;
}

.content {
  padding: 0 18px;
  max-height: 0;
  overflow: hidden;
  transition: max-height 0.2s ease-out;
  background-color: #f1f1f1;
}
h1{
  text-align: center;
}
li {
  text-align: left;
}
</style>
</head>
<body>
<button type="submit" class="logout_btn" onclick="location.href='/main.php';">Back</button>
<h1>COVID-19 CENTER</h1>

<div class="card">     
      <h3 style="text-align: center;">FREQUENTLY ASK QUESTIONS</h3>
        <button class="collapsible">HOW CAN I PROTECT MYSELF FROM COVID-19?</button>
        <div class="content">
          <ul>
          <li> Wear face mask and face shield </li>
          <li> Sanitize your hands </li>
          <li> Practice one-meter physical distancing and limit physical interaction </li>
          <li> Ensure good indoor ventilation and air flow </li>
          </ul>
        </div>
        <button class="collapsible">WHO ARE CONSIDERED AS CLOSE CONTACTS IN THE CONTEXT OF COVID-19?</button>
        <div class="content">
          <ul>
          <li> Interacted with a person with COVID-19 within one meter for more than 15 minutes </li>
          <li> Had direct physical interaction with probable or confirmed COVID-19 case </li>
          <li> Had interaction with a person with COVID-19 without wearing protective equipment </li>
          </ul>
        </div>
        <button class="collapsible">WHAT SHOULD I DO IF I AM A CLOSE CONTACT?</button>
        <div class="content">
          <span>Get tested if you are a close contact. Tell your Barangay Health Emergency Response Team (BHERT) that you are a close contact. This is to inform your next steps: </span>
          <ul>
          <li> COVID-19 Testing </li>
          <li> Referral to the Temporary Treatment and Monitoring Facility (TTMF) or hospital (if needed) </li>
          </ul>
        </div>
        <button class="collapsible">WHAT TESTS WILL BE USED?</button>
        <div class="content">
          <ul>
          <li> RT-PCR (gold standard) </li>
          <li> Antigen (for those specified places with rising cases of COVID-19, wherein RT-PCR tests are lacking) </li>
          </ul>
        </div>
        <button class="collapsible">WHAT IS THE DIFFERENCE BETWEEN QUARANTINE AND ISOLATION?</button>
        <div class="content">
          <span><b> Quarantine </b> - period to monitor well-being after being identified as a close contact with a person with COVID-19</span>
          <span><br><b> Isolation </b>- separating people with symptoms or confirmed COVID-19 cases</span>
        </div>
        <button class="collapsible">I HAVE TO ISOLATE MYSELF, WHAT SHOULD I DO?</button>
        <div class="content">
          <ul>
          <li> Continue wearing your face mask to prevent the spread of any virus/disease </li>
          <li> Disinfect all objects and surfaces that are frequently touched </li>
          <li> Practice physical distance and stay in your room </li>
          </ul>
        </div>
        <button class="collapsible">HOW IS THE HOME QUARANTINE OR ISOLATION BEING DONE?</button>
        <div class="content">
          <ol type="1">
            <li> If you have severe or critical symptoms, you will be referred to a hospital </li>
            <li> If you are asymptomatic or with mild/moderate symptoms, you may isolate yourself at your home or you may go to a Temporary Treatment and Monitoring Facility (TTMF) </li>
          </ol>
          <span> You may only isolate yourself at home if:</span>
          <ul>
          <li> Continue wearing your face mask to prevent the spread of any virus/disease </li>
          <li> Disinfect all objects and surfaces that are frequently touched </li>
          <li> Practice physical distance and stay in your room </li>
          </ul>
        </div>
        <button class="collapsible">WHAT SHOULD I REMEMBER IF I AM DOING HOME QUARANTINE/ISOLATION?</button>
        <div class="content">
          <span> Ensure to not spread COVID-19 to others!</span><br>
          <ul>
          <li> As much as possible, isolate yourself from other people at your home </li>
          <li> If you need to interact with other people or you need to go outside, wear face mask and practice one meter physical distancing </li>
          <li> Remind your carers or family members to always wear face mask if they have to interact with you </li>
          <li> Cover your mouth and nose using tissue when you cough or sneeze and wash your hands afterwards </li>
          <li> Do not let others use your personal belongings such as towels, bed sheets, plates, and utensils </li>
          </ul>
        </div>
       <button class="collapsible">I WANT TO QUARANTINE/ISOLATE IN A TEMPORARY TREATMENT AND MONITORING FACILITY (TTMF), WHAT SHOULD I DO?</button>
        <div class="content">
          <span> If you choose to go to a TTMF, contact your Barangay Health Emergency Response Team (BHERT) for your referral.</span><br><br>
        </div>
        <button class="collapsible">HOW WILL THE HOME MANAGEMENT OF MILD OR MODERATE SYMPTOMS BE DONE?</button>
        <div class="content">
          <span> If you have fever, you may do the following: </span><br>
          <ul>
          <li> Check temperature every four (4) hours; You may drink paracetamol if your temperature reaches above 37.5 degree celsius, every four (4) hours. </li>
          <li> Take a bath daily if you can and if possible </li>
          <li> Ensure good ventilation and airflow in your room </li>
          <li> Do not wear more layers of clothes </li>
          <li> Drink a lot of water, fresh fruit juices, and mild teas </li>
          </ul><br>
          <span> If you have cough or sore throat, you do the following: </span><br>
          <ul>
          <li> Make sure to drink your prescribed medicines </li>
          <li> Drink a lot of water </li>
          <li> Keep away from those that can heighten your symptoms such as dust, pollen, perfume, and animal fur </li>
          </ul>
        </div>
        <button class="collapsible">WHAT ARE THE SYMPTOMS THAT YOU NEED TO WATCH OUT FOR?</button>
        <div class="content">
          <ul>
          <li> Difficulty in breathing, even when sitting </li>
          <li> Cough, fever, and difficulty in breathing </li>
          <li> Severe coughing </li>
          <li> Confusion or sudden change in mental well-being </li>
          <li> Pain in the chest </li>
          <li> Low oxygen level </li>
          <li> Excessive sleepiness or cannot be woken up </li>
          <li> Bluish or darkened face or lips </li>
          <span> If you are experiencing other severe symptoms, call your BHERT immediately. </span>
          </ul>
        </div>
        <button class="collapsible">DO I NEED TO FINISH MY 14-DAY QUARANTINE IF I AM A CLOSE CONTACT THAT TESTED NEGATIVE FOR COVID-19?</button>
        <div class="content">
          <span> Yes. You need to finish your 14-day quarantine and finish it without developing any symptoms. </span>
          <h3>ASYMPTOMATIC</h3>
          <ul>
          <li> Isolation period: At least ten (10) days, from the day you received your positive result </li>
          <li> Where to isolate: Home or in a Temporary Treatment and Monitoring Facility (TTMF) </li>
          <li> Clearance: If you remain not having any symptoms within ten (10) days from the day you got tested </li>
          <li> Do you need to get re-tested? No </li>
          </ul>
          <h3>MILD</h3>
          <ul>
          <li> Isolation period: At least ten (10) days, from the day you received your positive result </li>
          <li> Where to isolate: Home or in a Temporary Treatment and Monitoring Facility (TTMF) </li>
          <li> Clearance: If you remain not having any symptoms and are clinically recovered
           in the past three (3) days </li>
          <li> <b> Do you need to get re-tested?</b> No </li>
          </ul>
           <h3>MODERATE, SEVERE O CRITICAL</h3>
          <ul>
          <li> Isolation period: At least twenty one (21) days, from the first day you experience any symptoms </li>
          <li> Where to isolate: Hospital </li>
          <li> Clearance: If you remain not having any symptoms and are clinically 
          recovered in the past three (3) days </li>
          <li> <b> Do you need to get re-tested?</b> No </li>
          <span> *home isolation/quarantine may only be done if you have your own room and bathroom/comfort room, and you do not live with a family member who belong to the vulnerable group</span>
          </ul>
        </div>
        <button class="collapsible">WHAT ARE THE WAYS TO MAINTAIN A STRONG PHYSICAL AND MENTAL WELL-BEING?</button>
        <div class="content">
          <ul>
          <li> Eat nutritious food such as fruits, vegetables, fish, and meat </li>
          <li> Exercise inside for 30 minutes per day </li>
          <li> Ensure to have enough sleep </li>
          <li> Practice self-care. Simple relaxing and talking with your loved ones is a form of self-care. </li>
          <li> Talk and check on with your loved ones. </li>
          </ul>
        </div>


</div>
<button type="submit" class="logout_btn" onclick="topFunction()">Back to top</button><br><br>
<script>
var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.maxHeight){
      content.style.maxHeight = null;
    } else {
      content.style.maxHeight = content.scrollHeight + "px";
    } 
  });
}

function topFunction() {
  document.body.scrollTop = 0;
  document.documentElement.scrollTop = 0;
}
</script>

</body>
</html>
)=====";

const char ERROR_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<head>
<title>SHIELD | Page Not Found</title>
<style>
	
    * { box-sizing: border-box; }
    body { font-family: Arial, Helvetica, sans-serif; }

    /* Style the counter cards */
    .card {
        border-radius: 10px;
        box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
        padding: 16px;
        text-align: center;
        background-color: #f1f1f1;
        border: none;
        
        margin-top: 175px;
        margin-bottom: 10px; /* Added */
        height: 250px;
    }

	/* Modal Content/Box */
	.modal-content {
		background-color: #fefefe;
		margin: 5% auto 5% auto; /* 5% from the top, 15% from the bottom and centered */
		border-radius: 10px;
		width: 90%; /* Could be more or less, depending on screen size */
	}

	/* Header */
	h1 {
    	color: #696969;
        font-size: 72px;
		margin-top: 30px;
		text-align: center;
        margin-bottom: 0px;
        
	}
    
    h2 {
        font-size: 32px;
		text-align: center;
	}

</style>
</head>

<body>

<form class="modal-content">
<div class="card">
  <span>
  	<h1>404</h1>
    <h2>Page not found.</h2>
  </span>
  </div>
</form>
</body>
</html>
)=====";
#endif