import java.net.*;
import java.io.*;

public class QuoteClient
{
	public static void main(String[] args)
	{
		try
		{
			//Make connection to server socket:
			String host = "127.0.0.1";
			int port = 6017;
			Socket sock = new Socket(host, port);
			InputStream in = sock.getInputStream();
			BufferedReader bin = new BufferedReader(new InputStreamReader(in));

			//Read the quote from the socket:
			String line;
			while ((line = bin.readLine()) != null) System.out.println(line);

			//Close the socket connection:
			sock.close();
		}
		catch(IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}
