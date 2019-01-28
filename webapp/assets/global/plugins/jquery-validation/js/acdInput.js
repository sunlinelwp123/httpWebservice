/**
 * 验证身份证是否有效
 * 身份证15位编码规则 : dddddd yymmdd xx p
 * dddddd : 地区码
 * yymmdd : 出生年月日
 * xx : 顺序类编码，无法确定
 * p : 性别, 奇数为男，偶数为女
 * <p />
 * 身份证18位编码规则 : dddddd yyyymmdd xxx y
 * dddddd : 地区码
 * yyyymmdd : 出生年月日
 * xxx : 顺序类编码，无法确定,奇数为男，偶数为女
 * p : 验证码,该位数值可通过前17位计算获得
 * <p />
 * 校验位计算公式：Y_P = mod(∑(Ai * Wi), 11)
 * i为身份证号码从右往左数的 2...18位; Y_P为校验码所在校验码数组位置
 * 
 */
var Wi = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2, 1];	//加权因子
var ValideCode = [1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2];	//身份证验证位值 10代表X
function IdCardValidate(idCard){
	idCard = trim(idCard.replace(/ /g, ""));
	if(idCard.length == 15){
		return isValidityBirthBy15IdCard(idCard);
	}else if(idCard.length == 18){
		var a_idCard = idCard.split("");//得到身份证数组
		if(isValidityBirthBy18IdCard(idCard) && isTrueValidateCodeBy18IdCard(a_idCard)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

//18位身份证验证
function isTrueValidateCodeBy18IdCard(a_idCard){
	var sum = 0; //声明加权求和变量
	if(a_idCard[17].toLowerCase() == 'x'){
		a_idCard[17] = 10;	//将最后位为x的验证码替换成10方便后续操作
	}
	for(var i = 0; i < 17; i++){
		sum+=Wi[i] * a_idCard[i];	//加权求和
	}
	valCodePosition = sum % 11;	//得到验证码所在位置
	if(a_idCard[17] == ValideCode[valCodePosition]){
		return true;
	}else{
		return false;
	}
}

//验证18位数身份证号码中的生日是否是有效生日
function isValidityBirthBy18IdCard(idCard18){
	var year = idCard18.substring(6,10);
	var month = idCard18.substring(10,12);
	var day = idCard18.substring(12,14);
	var temp_date = new Date(year, parseFloat(month)-1, parseFloat(day));
	
	if(temp_date.getFullYear() != parseFloat(year) 
			|| temp_date.getMonth() != parseFloat(month) -1 
			|| temp_date.getDate() != parseFloat(day)){
		return false;
	} else{
		return true;
	}
}

//验证15位数身份证号码中的生日是否是有效生日
function isValidityBirthBy15IdCard(idCard15){
	var year = idCard15.substring(6, 8);
	var month = idCard15.substring(8, 10);
	var day = idCard15.substring(10, 12);
	var temp_date = new Date(year, parseFloat(month)-1, parseFloat(day));
	
	if(temp_date.getFullYear() != parseFloat(year) 
			|| temp_date.getMonth() != parseFloat(month) -1 
			|| temp_date.getDate() != parseFloat(day)){
		return false;
	} else{
		return true;
	}
}

//获取出生日期
function getBirthDay(idCard){
	var birth = "";
	idCard = trim(idCard.replace(/ /g, ""));
	if(idCard.length == 18){
		var year = idCard.substring(6,10);
		var month = idCard.substring(10,12);
		var day = idCard.substring(12,14);
		birth = year + month + day;
	}else if(idCard.length == 15){
		var year = idCard.substring(6, 8);
		var month = idCard.substring(8, 10);
		var day = idCard.substring(10, 12);
		birth = year + month + day;
	}
	return birth;
}

/**
 * 通过身份证号码判断男女性别
 * @param idCard 15/18位身份证
 * @return F-女 M-男
 */
function mOrFByIdCard(idCard){
	idCard = trim(idCard.replace(/ /g, ""));
	if(idCard.length == 15){
		if(idCard.substring(14,15)%2 == 0){
			return "F";
		}else{
			return "M";
		}
	}else if(idCard.length == 18){
		if(idCard.substring(14,17)%2 == 0){
			return "F";
		}else{
			return "M";
		}
	}else{
		return null;
	}
}

//去掉字符串头尾空格
function trim(str){
	return str.replace(/(^\s*)|(\s*$)/g, "");
}

