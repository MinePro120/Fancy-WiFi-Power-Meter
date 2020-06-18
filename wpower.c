#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

unsigned long long int counter = 1;


void handleSignal ()
{
  system ("clear");
  exit (0);
}


void checkNic (char *nic)
{
  char str[100] = "cat /proc/net/wireless | grep ";
  strcat (str, nic);
  strcat (str, " > /dev/null");
  if (system (str) != 0)
    {
      system ("clear");
      fprintf (stderr, "\e[0m\e[1;91mNo such wireless interface/Disconnected\n\e[0m");
      exit (1);
    }
}


void printEssid (char *nic, char *buf)
{
  char str[100] = "iwgetid | grep -w ";
  strcat (str, nic);
  strcat (str, " | grep -E -o '\"(.*?)\"'");
  FILE *fp;
  fp = popen (str, "r");
  char essid[34];
  fgets (essid, sizeof (essid), fp);
  pclose (fp);
  char tempStr[50];
  sprintf (tempStr, "\e[1;93m ESSID : %s\e[0m", essid);
  strcat (buf, tempStr);
}


void printRssi (char *nic, char *buf)
{
  char str[100] = "cat /proc/net/wireless | grep -w ";
  strcat (str, nic);
  strcat (str, " | cut -z -d '.' -f 2");
  FILE *fp;
  fp = popen (str, "r");
  char powerStr[10];
  fgets (powerStr, sizeof (powerStr), fp);
  pclose (fp);
  char tempStr[400];
  int power = atoi (powerStr);
  static unsigned long long int sum = 0;
  sum += -power;
  float avg = (float) sum / counter;
  sprintf (tempStr, "\e[1;93m Power : %d dBm (-%1.1f avg.)\n\n\e[0m", power, avg);
  strcat (buf, tempStr);
  //printPower (rssi, buf);
  strcpy (tempStr, "");
  if (power >= -30)    // Print ascii art
    {
      strcat (tempStr, "\e[1;92m                         __\n");
      strcat (tempStr, "                      __|  |\n");
      strcat (tempStr, " ,--..Y            __|  |  |\n");
      strcat (tempStr, " \\   /`.        __|  |  |  |\n");
      strcat (tempStr, "  \\.    \\    __|  |  |  |  |\n");
      strcat (tempStr, "   \"\"\"--'   |  |  |  |  |  |");
    }
  else if (power >= -65)
    {
      strcat (tempStr, "\e[1;90m                         __\n");
      strcat (tempStr, "\e[1;92m                      __\e[1;90m|  |\n");
      strcat (tempStr, "\e[1;92m ,--..Y            __|  |\e[1;90m  |\n");
      strcat (tempStr, "\e[1;92m \\   /`.        __|  |  |\e[1;90m  |\n");
      strcat (tempStr, "\e[1;92m  \\.    \\    __|  |  |  |\e[1;90m  |\n");
      strcat (tempStr, "\e[1;92m   \"\"\"--'   |  |  |  |  |\e[1;90m  |");
    }
  else if (power >= -70)
    {
      strcat (tempStr, "\e[1;90m                         __\n");
      strcat (tempStr, "\e[1;90m                      __|  |\n");
      strcat (tempStr, "\e[1;92m ,--..Y            __\e[1;90m|  |  |\n");
      strcat (tempStr, "\e[1;92m \\   /`.        __|  |\e[1;90m  |  |\n");
      strcat (tempStr, "\e[1;92m  \\.    \\    __|  |  |\e[1;90m  |  |\n");
      strcat (tempStr, "\e[1;92m   \"\"\"--'   |  |  |  |\e[1;90m  |  |");
    }
  else if (power >= -80)
    {
      strcat (tempStr, "\e[1;90m                         __\n");
      strcat (tempStr, "\e[1;90m                      __|  |\n");
      strcat (tempStr, "\e[1;92m ,--..Y\e[1;90m            __|  |  |\n");
      strcat (tempStr, "\e[1;92m \\   /`.        __\e[1;90m|  |  |  |\n");
      strcat (tempStr, "\e[1;92m  \\.    \\    __|  |\e[1;90m  |  |  |\n");
      strcat (tempStr, "\e[1;92m   \"\"\"--'   |  |  |\e[1;90m  |  |  |");
    }
  else
    {
      strcat (tempStr, "\e[1;90m                         __\n");
      strcat (tempStr, "\e[1;90m                      __|  |\n");
      strcat (tempStr, "\e[1;92m ,--..Y\e[1;90m            __|  |  |\n");
      strcat (tempStr, "\e[1;92m \\   /`.\e[1;90m        __|  |  |  |\n");
      strcat (tempStr, "\e[1;92m  \\.    \\    __\e[1;90m|  |  |  |  |\n");
      strcat (tempStr, "\e[1;92m   \"\"\"--'   |  |\e[1;90m  |  |  |  |");
    }
  strcat (tempStr, "\e[0m\n");
  strcat (buf, tempStr);
}


int main (int argc, char **argv)
{
  signal (SIGINT, handleSignal);
  signal (SIGQUIT, handleSignal);
  if (argc != 2)
    {
      fprintf (stderr, "\e[1;91mUsage : wpower [WIRELESS INTERFACE]\n\e[0m");
      exit (1);
    }
  const char logo[] = "\e[1;94m\n __      ___     ___ _   ___                      __  __     _\n"
  " \\ \\    / (_)___| __(_) | _ \\_____ __ _____ _ _  |  \\/  |___| |_ ___ _ _\n"
  "  \\ \\/\\/ /| |___| _|| | |  _/ _ \\ V  V / -_) '_| | |\\/| / -_)  _/ -_) '_|\n"
  "   \\_/\\_/ |_|   |_| |_| |_| \\___/\\_/\\_/\\___|_|   |_|  |_\\___|\\__\\___|_|\n\n\e[0m";
  while (1)
    {
      char outBuf[900];    // Output buffer
      char infoBuf[600];    // Info buffer
      char tempStr[50] = "";
      sprintf (tempStr, "\e[1;93m Interface : %s\n\e[0m", argv[1]);
      strcpy (infoBuf, tempStr);    // Store interface to buffer
      printEssid (argv[1], infoBuf);
      printRssi (argv[1], infoBuf);
      checkNic (argv[1]);    // Check if NIC is still up
      strcpy (outBuf, logo);
      system ("clear");    // Clear screen
      printf (strcat (outBuf, infoBuf));    // Print everything
      sleep (1);    // Refresh delay
      counter++;
    }
  return 0;
}
