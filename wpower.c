#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


void handleSignal ()
{
  printf ("\033c");
  exit (0);
}


void printLogo (char *buf)
{
  char tempStr[500];
  strcpy (tempStr, "\e[1;34m\n __      ___     ___ _   ___                      __  __     _\n");
  strcat (tempStr, " \\ \\    / (_)___| __(_) | _ \\_____ __ _____ _ _  |  \\/  |___| |_ ___ _ _\n");
  strcat (tempStr, "  \\ \\/\\/ /| |___| _|| | |  _/ _ \\ V  V / -_) '_| | |\\/| / -_)  _/ -_) '_|\n");
  strcat (tempStr, "   \\_/\\_/ |_|   |_| |_| |_| \\___/\\_/\\_/\\___|_|   |_|  |_\\___|\\__\\___|_|\n\n\e[0m");
  strcat (buf, tempStr);
}


void printPower (int power, char *buf)
{
  char tempStr[200] = "\e[1;32m";
  if (power >= -20)
    {
      strcat (tempStr, "                         __\n");
      strcat (tempStr, "                      __|  |\n");
      strcat (tempStr, " ,--..Y            __|  |  |\n");
      strcat (tempStr, " \\   /`.        __|  |  |  |\n");
      strcat (tempStr, "  \\.    \\    __|  |  |  |  |\n");
      strcat (tempStr, "   \"\"\"--'   |  |  |  |  |  |");
    }
  else if (power >= -40)
    {
      strcat (tempStr, "                      __\n");
      strcat (tempStr, " ,--..Y            __|  |\n");
      strcat (tempStr, " \\   /`.        __|  |  |\n");
      strcat (tempStr, "  \\.    \\    __|  |  |  |\n");
      strcat (tempStr, "   \"\"\"--'   |  |  |  |  |");
    }
  else if (power >= -60)
    {
      strcat (tempStr, " ,--..Y            __\n");
      strcat (tempStr, " \\   /`.        __|  |\n");
      strcat (tempStr, "  \\.    \\    __|  |  |\n");
      strcat (tempStr, "   \"\"\"--'   |  |  |  |");
    }
  else if (power >= -80)
    {
      strcat (tempStr, " ,--..Y\n");
      strcat (tempStr, " \\   /`.        __\n");
      strcat (tempStr, "  \\.    \\    __|  |\n");
      strcat (tempStr, "   \"\"\"--'   |  |  |");
    }
  else
    {
      strcat (tempStr, " ,--..Y\n");
      strcat (tempStr, " \\   /`.\n");
      strcat (tempStr, "  \\.    \\    __\n");
      strcat (tempStr, "   \"\"\"--'   |  |");
    }
  strcat (tempStr, "\e[0m\n");
  strcat (buf, tempStr);
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


void printEssid (char *nic, char *buf)
{
  char str[100] = "iwgetid | grep -w ";
  strcat (str, nic);
  strcat (str, " | egrep -o '\"(.*?)\"'");
  FILE *fp;
  fp = popen (str, "r");
  char essid[34];
  fgets (essid, sizeof (essid), fp);
  pclose (fp);
  char tempStr[50];
  sprintf (tempStr, "\e[1;33m ESSID : %s\e[0m", essid);
  strcat (buf, tempStr);
}


void printRssi (char *nic, char *buf)
{
  char str[100] = "cat /proc/net/wireless | grep -w ";
  strcat (str, nic);
  strcat (str, " | cut -d ' ' -f 7 | cut -z -c 1-3");
  FILE *fp;
  fp = popen (str, "r");
  char rssi[4];
  fgets (rssi, sizeof (rssi), fp);
  pclose (fp);
  char tempStr[50];
  sprintf (tempStr, "\e[1;33m RSSI : %s dBm\n\n\e[0m", rssi);
  strcat (buf, tempStr);
  printPower (atoi (rssi), buf);
}


int main (int argc, char **argv)
{
  signal (SIGINT, handleSignal);
  signal (SIGQUIT, handleSignal);
  if (argc != 2)
    {
      fprintf (stderr, "\e[1;31mUsage : wpower [WIRELESS INTERFACE]\n\e[0m");
      exit (1);
    }
  checkNic (argv[1]);
  while (1)
    {
      char buf[500] = "";    // Output buffer
      printLogo (buf);
      char tempStr[50] = "";
      sprintf (tempStr, "\e[1;33m Interface : %s\n\e[0m", argv[1]);
      strcat (buf, tempStr);
      printEssid (argv[1], buf);
      printRssi (argv[1], buf);
      checkNic (argv[1]);    // Check if NIC is still up
      printf ("\033c");    // Clear screen
      printf (buf);    // Print everything
      sleep (1);    // Refresh delay
    }
  exit (0);
}
