

#include <stdio.h>
#include <time.h>
#include <string.h>   /* for check_user_string() */
#include <ctype.h>    /* for check_user_string() */
#include <sys/types.h> /* for check_user_string() */
#include <pwd.h>        /* for check_user_string() */

#include "hack.h"
#include "extern.h"

static boolean wiz_error_flag = FALSE;

void
sethanguphandler(handler)
void FDECL((*handler), (int));
{
#ifdef SA_RESTART
    /* don't want reads to restart.  If SA_RESTART is defined, we know
     * sigaction exists and can be used to ensure reads won't restart.
     * If it's not defined, assume reads do not restart.  If reads restart
     * and a signal occurs, the game won't do anything until the read
     * succeeds (or the stream returns EOF, which might not happen if
     * reading from, say, a window manager). */
    struct sigaction sact;

    (void) memset((genericptr_t) &sact, 0, sizeof sact);
    sact.sa_handler = (SIG_RET_TYPE) handler;
    (void) sigaction(SIGHUP, &sact, (struct sigaction *) 0);
#ifdef SIGXCPU
    (void) sigaction(SIGXCPU, &sact, (struct sigaction *) 0);
#endif
#else /* !SA_RESTART */
    (void) signal(SIGHUP, (SIG_RET_TYPE) handler);
#ifdef SIGXCPU
    (void) signal(SIGXCPU, (SIG_RET_TYPE) handler);
#endif
#endif /* ?SA_RESTART */
}

struct passwd* get_unix_pw()
{
    char *user;
    unsigned uid;
    static struct passwd *pw = (struct passwd *) 0;

    if (pw)
        return pw; /* cache answer */

    uid = (unsigned) getuid();
    user = getlogin();
    if (user) {
        pw = getpwnam(user);
        if (pw && ((unsigned) pw->pw_uid != uid))
            pw = 0;
    }
    if (pw == 0) {
        user = nh_getenv("USER");
        if (user) {
            pw = getpwnam(user);
            if (pw && ((unsigned) pw->pw_uid != uid))
                pw = 0;
        }
        if (pw == 0) {
            pw = getpwuid(uid);
        }
    }
    return pw;
}

boolean check_user_string(char* optstr)
{
    struct passwd *pw = get_unix_pw();
    int pwlen;
    char *eop, *w;
    char *pwname;

    if (optstr[0] == '*')
        return TRUE; /* allow any user */
    if (!pw)
        return FALSE;
    if (sysopt.check_plname)
        pwname = plname;
    else
        pwname = pw->pw_name;
    pwlen = strlen(pwname);
    eop = eos(optstr);
    w = optstr;
    while (w + pwlen <= eop) {
        if (!*w)
            break;
        if (isspace(*w)) {
            w++;
            continue;
        }
        if (!strncmp(w, pwname, pwlen)) {
            if (!w[pwlen] || isspace(w[pwlen]))
                return TRUE;
        }
        while (*w && !isspace(*w))
            w++;
    }
    return FALSE;
}

boolean authorize_wizard_mode()
{
    struct passwd *pw = get_unix_pw();

    if (pw && sysopt.wizards && sysopt.wizards[0]) {
        if (check_user_string(sysopt.wizards))
            return TRUE;
    }
    wiz_error_flag = TRUE; /* not being allowed into wizard mode */
    return FALSE;
}

int main(int argc, char** argv){
  char funct[100], arg1[100], arg2[100];

  memset(funct, '\0', 100);
  strncpy(funct, argv[1], 100);

  printf("%s", funct);
  for(int i = 2; i < argc; i++) {
    printf(" %s", argv[i]);
  }

  char* res = NULL;
  if(strncmp(funct, "makeplural", 100) == 0) {
    res = makeplural(argv[2]);
  }
  else if(strncmp(funct, "makesingular", 100) == 0) {
    res = makesingular(argv[2]);
  }
  else {
    res = "Function not found";
  }


  time_t  current_time;
  char*   ctime_string; /* timestamp string */
  current_time = time(NULL);
  ctime_string = ctime(&current_time);
  if(ctime_string == NULL) printf("FAILURE TO RETRIEVE TIME\n");
  /* printf("Current date/time is: %s\n", ctime_string ); */

  printf(" %s", res);
  //fprintf (fp, "Test Log: %s\nTested Item: %s\n", ctime_string, tested_item);

  /* for - num parameters/tests */
  /* print ( "message:  ", function calls  ); */

  return 0;

}
