function unescape_opera_br(text)
{
	if(typeof(RegExp) == 'function') {
                re = /quot;/g;
                newstr=text.replace(re, '');
                re = /&/g;
                return newstr.replace(re, '\"');
        }
        else return text;
}