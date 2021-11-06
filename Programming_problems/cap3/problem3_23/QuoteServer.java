//Server:

import java.net.*;
import java.io.*;
import java.util.*;

public class QuoteServer
{
	public static void main(String[] args)
	{
		int server_port = 6017;
		Calendar currentCalendar = Calendar.getInstance();
		System.out.println("Starting server: ");

		try
		{
			ServerSocket sock = new ServerSocket(server_port);

			//Listen for connections:
			while(true)
			{
				System.out.println("Listening for connections -> ");
				Socket client = sock.accept();
				System.out.println("- Quote requested.");
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

				//Write the quote to the socket:
				String quote = getQuote(currentCalendar.get(currentCalendar.DAY_OF_YEAR));
				pout.println(quote);
				System.out.println("- Quote sent: " + quote);

				//Close the socket:
				client.close();
			}

		}
		catch(IOException ioe)
		{
			System.err.println(ioe);
		}
	}

	private static String getQuote(int n)
	{
		//Quotes from: parade.com
		String[] quotes = {
			           "Hello, World! - Computer",
		                   "The pusrpose of our lives is to be happy. - Dali Lama",
		                   "Life is what happens when you're busy making other plans. - John Lennon",
				   "Get busy living or get busy dying. - Stephen King",
				   "If you want to live a happy life, tie it to a goal, not to people or things. - Albert Einstein",
				   "Money and success don't change people; they merely amplify what is already there - Will Smith",
				   "Life is really simple, but men insist on making it complicated. - Confucius."
		                  };

		int index = n % quotes.length;
		return quotes[index];
	}

}
