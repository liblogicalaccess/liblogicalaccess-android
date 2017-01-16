package com.islog.liblogicalaccess;


import android.nfc.tech.IsoDep;
import android.util.Log;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Created by Adrien on 23/01/14.
 */
public class IsoDepCommand {

    private static IsoDep myCard = null;
    private static String mycardType = "";
    private static final Object lock = new Object();

    //private static final byte[] DF_GET_VERSION = new byte[]{(byte)0x90, (byte)0x60, (byte)0x00, (byte)0x00, (byte)0x00};
    //private static final byte[] DF_ADDITIONAL_FRAME = new byte[]{(byte)0x90, (byte)0xAF, (byte)0x00, (byte)0x00, (byte)0x00};

    public static void setMyCard(IsoDep card, String cardType)
    {
        synchronized (lock) {
            myCard = card;
            mycardType = cardType;

           /* try {
                if (cardType == "DESFire")
                {
                    myCard.setTimeout(1000);
                    myCard.connect();

                    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

                    byte[] result = myCard.transceive(DF_GET_VERSION);
                    if (result.length >= 7)
                        outputStream.write(result, 0, 7);

                    if (result.length - 2 == 7 && result[result.length - 1] == DF_ADDITIONAL_FRAME[1])
                    {
                        result = myCard.transceive(DF_ADDITIONAL_FRAME);
                        if (result.length >= 7)
                            outputStream.write(result, 0, 7);
                    }

                    if (result.length - 2 == 7 && result[result.length - 1] == DF_ADDITIONAL_FRAME[1])
                    {
                        result = myCard.transceive(DF_ADDITIONAL_FRAME);
                        if (result.length >= 7)
                            outputStream.write(result, 0, 7);
                    }

                    byte[] cardVersion = outputStream.toByteArray();

                    if (cardVersion.length >= 11 && cardVersion[11] >= 0x01)
                    {
                        cardType = "DESFireEV1";
                        // myActivity.changeCardType(cardType);
                    }

                    if (cardVersion.length >= 21)
                    {
                        myCsn = new byte[7];
                        System.arraycopy(cardVersion, 14, myCsn, 0, 7);

                        StringBuilder sb = new StringBuilder();
                        for (byte b: myCsn)
                            sb.append(String.format("%02X", b));
                        Log.d("NFCAndroid", "CSN: " + sb.toString());
                    }
                    else
                        Log.d("NFCAndroid", "CSN Not detected.");

                    myCard.close();
                }

            } catch (Exception e) {
                removeCard();
                e.printStackTrace();
            }*/
        }
    }

    public static String getMycardType() throws Exception
    {
        synchronized (lock) {
            return mycardType;
        }
    }

    /*public static String getMyCSN() throws Exception
    {
        synchronized (lock) {
            if (myCsn == null)
                throw new Exception("CSN Not detected.");

            StringBuilder sb = new StringBuilder();
            for (byte b: myCsn)
                sb.append(String.format("%02X", b));

            return sb.toString();
        }
    }*/

    public static boolean connect()
    {
        synchronized (lock) {
            if (myCard == null)
                return false;
            try {
                if (!myCard.isConnected())
                {
                    myCard.setTimeout(1000);
                    myCard.connect();
                }
            } catch (IOException e) {
                removeCard();
                return false;
            }
            return true;
        }
    }

    public static void disconnect() throws Exception
    {
        synchronized (lock) {
            if (myCard == null)
                return;
            try {
                myCard.close();
                removeCard();
            } catch (IOException e) {
                removeCard();
                throw e;
            }
        }
    }

    public static byte[] execCommand(byte[] cmd) throws IOException {
        synchronized (lock) {
            if (myCard == null)
                return null;
            try {
                return myCard.transceive(cmd);
            } catch (IOException e) {
                removeCard();
                throw e;
            }
        }
    }

    public static void removeCard()
    {
        myCard = null;
        mycardType = "";
    }
}

