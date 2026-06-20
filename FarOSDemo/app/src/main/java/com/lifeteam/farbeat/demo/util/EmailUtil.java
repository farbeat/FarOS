package com.lifeteam.farbeat.demo.util;

import com.lifeteam.farbeat.util.log.LogUtil;
import com.lifeteam.farbeat.util.thread.ThreadPoolUtils;

import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.FileDataSource;
import javax.mail.Message;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;
import javax.mail.internet.MimeUtility;

public class EmailUtil {

	static class AuthenticatorA extends javax.mail.Authenticator {
		private final String strUser;
		private final String strPwd;

		public AuthenticatorA(String user, String password) {
			this.strUser = user;
			this.strPwd = password;
		}

		protected PasswordAuthentication getPasswordAuthentication() {
			return new PasswordAuthentication(strUser, strPwd);
		}
	}

	private static void sendMail(String fromMail, String fromPwd, String toMail, String host,
								String port, String title, String body, String filePath) {
		try {
			Properties props = System.getProperties();
			// 发送邮件的服务器的地址
			props.put("mail.smtp.host", host);
			// 发送邮件的服务器的端口
			props.put("mail.smtp.port", port);
			props.put("mail.smtp.auth", "true");
			AuthenticatorA aa = new AuthenticatorA(fromMail, fromPwd);
			Session session = Session.getDefaultInstance(props, aa);
			MimeMessage message = new MimeMessage(session);
			// 邮件发送者的地址
			message.setFrom(new InternetAddress(fromMail));

			message.addRecipient(Message.RecipientType.TO, new InternetAddress(toMail));
			message.setSubject(title);

			message.setText(MimeUtility.encodeWord(body));

			MimeMultipart allMultipart = new MimeMultipart("mixed");

			MimeBodyPart attachPart = new MimeBodyPart();
			FileDataSource fds = new FileDataSource(filePath);
			attachPart.setDataHandler(new DataHandler(fds));
			attachPart.setFileName(MimeUtility.encodeWord(fds.getName()));

			MimeBodyPart textBodyPart = new MimeBodyPart();
			textBodyPart.setText(body);

			allMultipart.addBodyPart(attachPart);
			allMultipart.addBodyPart(textBodyPart);
			message.setContent(allMultipart);
			message.saveChanges();
			Transport.send(message);
		} catch (Exception e) {
			LogUtil.E("Email sending exception!", e.toString());
		}
	}

    /**
     * 发送一封邮件
     */
    public static void sendEmail(final String fromMail, final String fromPwd, final String toMail, final String host,
								 final String port, final String title, final String body, final String filePath) {
        ThreadPoolUtils.getInstance().addTask(new Runnable() {
            @Override
            public void run() {
                EmailUtil.sendMail(fromMail, fromPwd, toMail, host, port, title, body, filePath);
            }
        });
    }
}