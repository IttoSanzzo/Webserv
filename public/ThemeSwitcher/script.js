
function getCSSVariable(variable) {
	return (getComputedStyle(document.documentElement).getPropertyValue(variable));
}
function setCSSVariable(variable, value) {
	document.documentElement.style.setProperty(variable, value);
}

function themeSwitch() {
	const themeButton = document.getElementById('themeButton');
	const stateTitle = document.getElementById('stateTitle');
	if (getCSSVariable("--theme") == "light") {
		setCSSVariable("--theme", "dark");
		setCSSVariable("--main-bg-color", "purple");
		setCSSVariable("--secondary-bg-color", "darkblue");
		themeButton.style.transform = "translateX(35px)"
		stateTitle.textContent = "It's Night";
	}
	else {
		setCSSVariable("--theme", "light");
		setCSSVariable("--main-bg-color", "yellow");
		setCSSVariable("--secondary-bg-color", "white");
		themeButton.style.transform = "unset"
		stateTitle.textContent = "It's Day";
	}
}