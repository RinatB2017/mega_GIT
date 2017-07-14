function onTextChanged( text ) {
    output.text = text.toUpperCase();
}

input.textChanged.connect( onTextChanged );
