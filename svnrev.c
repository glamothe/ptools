/*  SvnRev
 *
 *  This utility retrieves the highest number that follows the "$Id:�$" keyword
 *  or a combination of the $Rev:�$ and $Date:�$ keywords. The Subversion
 *  version control system expands these keywords and keeps them up to date.
 *  For an example of the tag, see the end of this comment.
 *
 *  Details on the usage and the operation of this utility is available on-line
 *  at http://www.compuphase.com/svnrev.htm.
 *
 *
 *  License
 *
 *  Copyright (c) 2005-2007, ITB CompuPhase (www.compuphase.com).
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 *
 *  Pursuant to #2 above: this version was modified by Tom McCann (tommc@spoken.com)
 *  to support output of .java files.
 *
 * Version: $Id: svnrev.c 3852 2007-11-26 10:37:51Z thiadmer $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svnrev.h"


#if defined __WIN32__ || defined _Win32 || defined _WIN32
  #define DIRSEP '\\'
#elif defined macintosh
  #define DIRSEP ':'
#else
  /* assume Linux/Unix */
  #define DIRSEP '/'
#endif


static void about(void)
{
  printf("svnrev 1.5." SVN_REVSTR "\n\n");
  printf("Usage: svnrev [-ofilename] [-i] [-jpackage] <input> [input [...]]\n\n"
         "The -o option sets the filename for the output file with the build number.\n"
         "When no filename follows \"-o\", the result is written to stdout. The default\n"
         "filename is \"svnrev.h\" for C/C++ and \"VersionInfo.java\" for Java.\n\n"
         "The -i option should be used when the file list is only a partial set of all\n"
         "files. With this option, svnrev also scans the output file that was generated\n"
         "on a previous run.\n\n"
         "The -j option writes a java package file instead of a C/C++ header file. The\n"
         "name of the package must follow the option.\n");
  exit(1);
}

static void processfile(const char *name, int failsilent,
                        int *max_build, int *accum_build,
                        int *max_year, int *max_month, int *max_day,
                        int *ismodified)

{
  char str[512], str_base[512];
  char name_base[512];
  char *p1;
  FILE *fp, *fp_base;
  int build, maj_build;
  int year, month, day;
  int cnt;
  char modchar;

  /* since we also want to verify whether the file is modified in version
   * control, get the path to the working copy name
   * for every source file "<path>\<filename>, the "working copy" base can
   * be found in "<path>\.svn\text-base\<filename>.svn-base"
   */
  if ((p1 = strrchr(name, DIRSEP)) != NULL) {
    ++p1; /* skip directory separator character ('\' in Windows, '/' in Linux) */
    strncpy(name_base, name, (int)(p1 - name));
    name_base[(int)(p1 - name)] = '\0';
  } else {
    name_base[0] = '\0';
    p1 = (char*)name;
  } /* if */
  sprintf(name_base + strlen(name_base), ".svn%ctext-base%c%s.svn-base",
          DIRSEP, DIRSEP, p1);

  /* first extract the revision keywords */
  fp = fopen(name, "r");
  if (fp == NULL) {
    if (!failsilent)
      fprintf(stderr, "Failed to open input file '%s'\n", name);
    return;
  } /* if */
  fp_base = fopen(name_base, "r");  /* fail silently */
  build = 0;
  maj_build = 0;      /* RCS / CVS */
  year = month = day = 0;

  while (fgets(str, sizeof str, fp) != NULL) {
    if (fp_base == NULL || fgets(str_base, sizeof str_base, fp_base) == NULL)
      str_base[0] = '\0';
    if ((p1 = strstr(str, "$Id:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Id: %*s %d %d-%d-%d", &build, &year, &month, &day) < 4
          && sscanf(p1, "$Id: %*s %d %d/%d/%d", &build, &year, &month, &day) < 4)
        if (sscanf(p1, "$Id: %*s %d.%d %d-%d-%d", &maj_build, &build, &year, &month, &day) < 5)
          sscanf(p1, "$Id: %*s %d.%d %d/%d/%d", &maj_build, &build, &year, &month, &day);
    } else if ((p1 = strstr(str, "$Rev:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Rev: %d.%d", &maj_build, &build) < 2) {
        sscanf(p1, "$Rev: %d", &build);
        maj_build = 0;
      } /* if */
    } else if ((p1 = strstr(str, "$Revision:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Revision: %d.%d", &maj_build, &build) < 2) {
        /* SvnRev also writes this keyword in its own generated file; read it
         * back for partial updates
         */
        cnt = sscanf(p1, "$Revision: %d%c", &build, &modchar);
        if (cnt == 2 && modchar == 'M' && ismodified != NULL)
          *ismodified = 1;
        maj_build = 0;
      } /* if */
    } else if ((p1 = strstr(str, "$Date:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Date: %d-%d-%d", &year, &month, &day) < 3)
        sscanf(p1, "$Date: %d/%d/%d", &year, &month, &day);
    } else if (ismodified != NULL && *ismodified == 0 && fp_base != NULL) {
      /* no keyword present, compare the lines for equivalence */
      *ismodified = strcmp(str, str_base) != 0;
    } /* if */

    if (maj_build)
      *accum_build += build;            /* RCS / CVS */
    else if (build > *max_build)
      *max_build = build;               /* Subversion */
    if (year > *max_year
        || (year == *max_year && month > *max_month)
        || (year == *max_year && month == *max_month && day > *max_day))
    {
        *max_year = year;
        *max_month = month;
        *max_day = day;
    } /* if */
    if (build > 0 && year > 0 && (fp_base == NULL || ismodified == NULL || *ismodified != 0))
      break;      /* both build # and date found, not comparing or modification
                   * already found => no need to search further */

  } /* while */
  fclose(fp);
  if (fp_base != NULL)
    fclose(fp_base);
}

int main(int argc, char *argv[])
{
  char *outname = NULL;
  FILE *fp;
  int index;
  int process_self = 0;
  int max_build, accum_build;
  int max_year, max_month, max_day;
  int ismodified;
  int write_java = 0;   /* flag for Java output, 0=.h output, 1=.java output */
  /* java package to put revision info in.
   * REVIEW - I assume if you want Java output you will specify a package. */
  char *java_package = NULL;

  if (argc <= 1)
    about();

  /* phase 1: scan through all files and get the highest build number */

  max_build = 0;
  accum_build = 0;      /* for RCS / CVS */
  max_year = max_month = max_day = 0;
  ismodified = 0;
  for (index = 1; index < argc; index++) {
    /* check for options */
    if (argv[index][0] == '-'
#if defined __WIN32__ || defined _Win32 || defined _WIN32
     || argv[index][0] == '/'
#endif
    )
    {
      switch (argv[index][1]) {
      case 'o':
        outname = &argv[index][2];
        continue;
      case 'i':
        process_self = 1;
        continue;
      case 'j':
        write_java=1;
        java_package = &argv[index][2];
        continue;
      default:
        fprintf(stderr, "Invalid option '%s'\n", argv[index]);
        about();
      } /* switch */
    } /* if */

    processfile(argv[index], 0, &max_build, &accum_build, &max_year, &max_month, &max_day, &ismodified);
  } /* for */

  if (outname == NULL)
    outname = write_java ? "SvnRevision.java" : "svnrev.h";

  /* also run over the existing header file, if any */
  if (process_self && *outname != '\0')
    processfile(outname, 1, &max_build, &accum_build, &max_year, &max_month, &max_day, NULL/*&ismodified*/);

  if (accum_build > max_build)
    max_build = accum_build;

  /* phase 2: write a file with this highest build number */
  if (*outname == '\0') {
    fp = stdout;
  } else if ((fp = fopen(outname, "w")) == NULL) {
    fprintf(stderr, "Failed to create output file '%s'\n", outname);
    return 2;
  } /* if */
  if (*outname != '\0') {
    /* don't print the comments to stdout */
    fprintf(fp, "/* This file was generated by the \"svnrev\" utility\n"
                " * (http://www.compuphase.com/svnrev.htm).\n"
                " * You should not modify it manually, as it may be re-generated.\n"
                " *\n");
    if (ismodified)
      fprintf(fp, " * $Revision: %dM$\n", max_build);
    else
      fprintf(fp, " * $Revision: %d$\n", max_build);
    fprintf(fp, " * $Date: %04d-%02d-%02d$\n"
                " */\n\n", max_year, max_month, max_day);
  } /* if */

  if (write_java) {
    if (java_package == NULL || *java_package == '\0')
      java_package = "com.compuphase";
    fprintf(fp, "package %s;\n\n", java_package);
    fprintf(fp, "public interface SvnRevision\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    public final static int SVN_REV = %d;\n", max_build);
    if (ismodified)
      fprintf(fp, "    public final static String SVN_REVSTR = \"%dM\";\n", max_build);
    else
      fprintf(fp, "    public final static String SVN_REVSTR = \"%d\";\n", max_build);
    fprintf(fp, "    public final static String SVN_REVDATE = \"%04d-%02d-%02d\";\n", max_year, max_month, max_day);
    fprintf(fp, "    public final static long SVN_REVSTAMP = %04d%02d%02dL;\n", max_year, max_month, max_day);
    fprintf(fp, "    public final static int SVN_MODIFIED = %d;\n", ismodified);
    fprintf(fp, "}\n\n");
  } else {
    fprintf(fp, "#ifndef SVNREH_H\n");
    fprintf(fp, "#define SVNREV_H\n\n");
    fprintf(fp, "#define SVN_REV\t\t%d\n", max_build);
    if (ismodified)
      fprintf(fp, "#define SVN_REVSTR\t\"%dM\"\n", max_build);
    else
      fprintf(fp, "#define SVN_REVSTR\t\"%d\"\n", max_build);
    fprintf(fp, "#define SVN_REVDATE\t\"%04d-%02d-%02d\"\n", max_year, max_month, max_day);
    fprintf(fp, "#define SVN_REVSTAMP\t%04d%02d%02dL\n", max_year, max_month, max_day);
    fprintf(fp, "#define SVN_MODIFIED\t%d\n", ismodified);
    fprintf(fp, "\n#endif /* SVNREV_H */\n");
  } /* if */

  if (*outname != '\0')
    fclose(fp);

  return 0;
}
