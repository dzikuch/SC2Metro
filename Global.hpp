#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define APPLICATION_NAME "FMetro"
#define APPLICATION_VERSION "2.0.0"

#define GAME_FILE_EXTENSION "fmg"
#define GAME_FILE_FILTER QString(tr("FMetro game (*.%1)")).arg(GAME_FILE_EXTENSION)
#define GAME_FILE_SIGNATURE "FMetro2 game file"
#define GAME_FILE_VERSION 1

#define FORMAT_DURATION_mm_ss(seconds) QString("%1:%2").arg(seconds / 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0'))

#endif // GLOBAL_HPP
