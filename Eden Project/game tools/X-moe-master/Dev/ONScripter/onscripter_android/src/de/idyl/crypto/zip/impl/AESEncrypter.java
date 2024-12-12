package de.idyl.crypto.zip.impl;

import java.util.zip.ZipException;

/**
 * Encrypt.
 *
 * @author olaf@merkert.de
 */
public interface AESEncrypter {

	public void init(String pwStr, int keySize) throws ZipException;

	public void encrypt(byte[] in, int length);

	public byte[] getSalt();

	public byte[] getPwVerification();

	public byte[] getFinalAuthentication();

}
