#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void printLogo ()
{
  printf ("\e[1;34m __      ___     ___ _   ___                      __  __     _\n");
  printf (" \\ \\    / (_)___| __(_) | _ \\_____ __ _____ _ _  |  \\/  |___| |_ ___ _ _\n");
  printf ("  \\ \\/\\/ /| |___| _|| | |  _/ _ \\ V  V / -_) '_| | |\\/| / -_)  _/ -_) '_|\n");
  printf ("   \\_/\\_/ |_|   |_| |_| |_| \\___/\\_/\\_/\\___|_|   |_|  |_\\___|\\__\\___|_|\n\e[0m");
}


void printPower (int power)
{
  printf ("\e[1;32m");
  if (power >= -20)
    {
      printf ("                         __\n");
      printf ("                      __|  |\n");
      printf (" ,--..Y            __|  |  |\n");
      printf (" \\   /`.        __|  |  |  |\n");
      printf ("  \\.    \\    __|  |  |  |  |\n");
      printf ("   \"\"\"--'   |  |  |  |  |  |\n");
    }
  else if (power >= -40)
    {
      printf ("                      __\n");
      printf (" ,--..Y            __|  |\n");
      printf (" \\   /`.        __|  |  |\n");
      printf ("  \\.    \\    __|  |  |  |\n");
      printf ("   \"\"\"--'   |  |  |  |  |\n");
    }
  else if (power >= -60)
    {
      printf (" ,--..Y            __\n");
      printf (" \\   /`.        __|  |\n");
      printf ("  \\.    \\    __|  |  |\n");
      printf ("   \"\"\"--'   |  |  |  |\n");
    }
  else if (power >= -80)
    {
      printf (" ,--..Y\n");
      printf (" \\   /`.        __\n");
      printf ("  \\.    \\    __|  |\n");
      printf ("   \"\"\"--'   |  |  |\n");
    }
  else
    {
      printf (" ,--..Y\n");
      printf (" \\   /`.\n");
      printf ("  \\.    \\    __\n");
      printf ("   \"\"\"--'   |  |\n");
    }
  printf ("\e[0m");
}


void checkNic (char *nic)
{
  char str[100] = "cat /proc/net/wireless | grep ";
  strcat (str, nic);
  strcat (str, " > /dev/null");
  if (system (str) != 0)
    {
      fprintf (stderr, "\e[0m\e[1;31mNo such wireless interface\n\e[0m");
      exit (1);
    }
}


void printEssid (char *nic)
{
  char str[100] = "iwgetid | grep -w ";
  strcat (str, nic);
  strcat (str, " | egrep -o '\"(.*?)\"'");
  printf ("\e[1;33m ESSID : ");
  fflush (stdout);
  system (str);
  printf ("\e[0m");
}


printRssi (char *nic, char str[])
{
  strcpy (str, "cat /proc/net/wireless | grep -w ");
  strcat (str, nic);
  strcat (str, " | cut -d ' ' -f 7 | cut -z -c 1-3");
  printf ("\e[1;33m RSSI : ");
  fflush (stdout);
  system (str);
  printf (" dBm\n\n\e[0m");
}


int main (int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf (stderr, "\e[1;31mUsage : wsignal [WIRELESS INTERFACE]\n\e[0m");
      exit (1);
    }
  checkNic (argv[1]);
  while (1)
    {
      printf ("\033c");
      putchar ('\n');
      printLogo ();
      putchar ('\n');
      printf ("\e[1;33m Interface : %s\n\e[0m", argv[1]);
      fflush (stdout);
      printEssid (argv[1]);
      FILE *fp;
      char str[100];
      printRssi (argv[1], str);
      fp = popen (str, "r");    // Stores RSSI as int
      char powerString[4];
      fgets (powerString, sizeof (powerString), fp);
      pclose (fp);
      int power = atoi (powerString);
      printPower (power);
      sleep (1);    // Refresh delay
      checkNic (argv[1]);    // Checks if NIC is still up
    }
  exit (0);
}
