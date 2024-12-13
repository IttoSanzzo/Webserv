const screenString = document.getElementById("screenString")
const lastNumberString = document.getElementById("lastNumberString")
let calcSymbol = "";

function computeButton(type) {
	switch (type) {
		case ('C'):
			reset();
			break;
		case ('/'):
		case ('*'):
		case ('-'):
		case ('+'):
			doSymbol(type);
			break;
		case ('='):
			doCalc();
			break;
		case ('1'):
		case ('2'):
		case ('3'):
		case ('4'):
		case ('5'):
		case ('6'):
		case ('7'):
		case ('8'):
		case ('9'):
		case ('0'):
			updateScreen(type);
			break;
		default:
			break;
	}
}

function updateScreen(type) {
	if (screenString.textContent == "0" || screenString.textContent == '/' || screenString.textContent == '*' || screenString.textContent == '-' || screenString.textContent == '+')
		screenString.textContent = type;
	else
		screenString.textContent += type;
}

function doSymbol(symbol) {
	if (!(screenString.textContent == '/' || screenString.textContent == '*' || screenString.textContent == '-' || screenString.textContent == '+'))
		lastNumberString.textContent = screenString.textContent;
	screenString.textContent = symbol;
	calcSymbol = symbol;
}
function doCalc() {
	screenString.textContent = eval(lastNumberString.textContent + calcSymbol + screenString.textContent);
	lastNumberString.textContent = "";
	symbol = "";
}

function reset() {
	screenString.textContent = "0";
	lastNumberString.textContent = "";
	symbol = "";
}

window.addEventListener('load', function () {
	this.document.addEventListener("keydown", (event) => {
		switch (event.key) {
			case ("c"):
			case ("Backspace"):
				computeButton('C');
			case ("Enter"):
				computeButton('=');
			default:
				computeButton(event.key);
				break;
		}
	})
})