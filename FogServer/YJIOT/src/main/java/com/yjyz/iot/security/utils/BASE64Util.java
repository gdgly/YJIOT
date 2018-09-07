package com.yjyz.iot.security.utils;

import java.util.Calendar;

import org.apache.commons.lang3.StringUtils;
import org.springframework.stereotype.Component;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

@Component
public class BASE64Util {
	public String getTokenDate() {
		Calendar car = Calendar.getInstance();
		StringBuffer sb = new StringBuffer();
		sb.append(String.valueOf(car.get(Calendar.YEAR)));
		sb.append(getLenTime(car.get(Calendar.MONTH) + 1));
		sb.append(getLenTime(car.get(Calendar.DATE)));
		sb.append(getLenTime(car.get(Calendar.HOUR_OF_DAY)));
		sb.append(getLenTime(car.get(Calendar.MINUTE)));
		sb.append(getLenTime(car.get(Calendar.SECOND)));
		return sb.toString();
	}

	public String getLenTime(int len) {
		String str = String.valueOf(len);
		if (len < 10) {
			return "0".concat(str);
		} else {
			return str;
		}
	}

	public String getToken(String data) {
		String tokenrId = getRandomString(3);
		return getBASE64(data.concat(getTokenDate()).concat(tokenrId));
	}

	public String getBASE64(String s) {
		if (s == null)
			return null;
		return (new BASE64Encoder()).encode(s.getBytes());
	}

	// 将 BASE64 编码的字符串 s 进行解码
	public String getFromBASE64(String s) {
		if (s == null)
			return null;
		BASE64Decoder decoder = new BASE64Decoder();
		try {
			byte[] b = decoder.decodeBuffer(s);
			return new String(b);
		} catch (Exception e) {
			return null;
		}
	}

	public static String getRandomString(int size) {
		char[] c = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
				'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm' };
		java.util.Random random = new java.util.Random(); // 初始化随机数产生器
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < size; i++) {
			sb.append(c[Math.abs(random.nextInt()) % c.length]);
		}
		return sb.toString();
	}

	public String getData(String token) {
		String strcon = getFromBASE64(token);
		return StringUtils.substring(strcon, 0, strcon.length() - 17);
	}

	public static void main(String[] args) {
		BASE64Util util = new BASE64Util();
		String data = "herolizhen";
		String token = util.getToken(data);
		System.out.println(token);
		System.out.println(util.getData(token));

	}

}