const os = require("os");
// получим имя текущего пользователя
let userName = os.userInfo().username;
 
console.log(userName);
