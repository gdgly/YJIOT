package com;

import java.security.Key;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Date;

import io.jsonwebtoken.Jwt;
import io.jsonwebtoken.JwtBuilder;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;

public class test {

	public static void main(String[] args) {
		test te = new test();
		
//		String aa = "asdfasdf{devName}";
//		String str = aa.replace("{devName}", "xxx");
		try {
			String pubKey = "xxx";
			String priKey = "xxx";
			byte[] privateKeyBytes = org.apache.commons.codec.binary.Base64.decodeBase64(priKey);
			byte[] pubKeyBytes = org.apache.commons.codec.binary.Base64.decodeBase64(pubKey);

			PrivateKey privateKey = te.getPrivateKey("RSA", privateKeyBytes);
			java.security.PublicKey publicKey = te.getPublicKey("RSA", pubKeyBytes);
			SignToken token1 = te.generateToken("E0000000000000000009", "101101-00000004", "13003618788", 7,
					SignatureAlgorithm.RS256, privateKey);
			String tokenString = token1.getValue();
			@SuppressWarnings("rawtypes")
			Jwt jwt = Jwts.parser().setSigningKey(publicKey).parse(tokenString);
			System.out.println(jwt.getHeader());
		} catch (Exception e) {

		}
	}

	public java.security.PublicKey getPublicKey(String KEY_ALGORITHM, byte[] pubKey)
			throws InvalidKeySpecException, NoSuchAlgorithmException {

		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(pubKey);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		return keyFactory.generatePublic(x509KeySpec);
	}

	public PrivateKey getPrivateKey(String KEY_ALGORITHM, byte[] priKey)
			throws NoSuchAlgorithmException, InvalidKeySpecException {
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(priKey);
		return keyFactory.generatePrivate(pkcs8KeySpec);
	}

	public SignToken generateToken(String mid, String docId, String devId, long delay, SignatureAlgorithm alg,
			Key signingKey) {
		SignToken token = new SignToken(mid, docId, devId);
		// 定义开始时间
		Date now = new Date();
		Date end = new Date(now.getTime() + delay * 24 * 60 * 60 * 1000);
		token.setCreateTime(now);
		token.setExpiredIn(end);

		// 构建一个Token
		JwtBuilder builder = Jwts.builder().setId(token.getId());
		builder.setIssuedAt(token.getCreateTime()); // Token颁布的时间
		builder.setExpiration(token.getExpiredIn()); // Token失效的时间

		builder.setIssuer(mid); // 发行者 授权者"ssqian.com.cn"
		builder.setSubject("hand-sign"); // 主题
		builder.setAudience(token.toJSONString()); // 观众
		builder.signWith(alg, signingKey);

		token.setValue(builder.compact());
		return token;
	}

}
