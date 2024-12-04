\version "2.24.4"
\header {
title = "Sight Reading Practice"
composer = "Juan P. Montiel"
}
\score 
{
\new PianoStaff \with { instrumentName = "Piano" } << 
\new Staff {
\tempo "Variable" 4 = 60-120
\transpose c c {
\key c \major
\time 4/4
\clef G
f'4 f'4 b'4 a'4 |
f'4 g'4 d'4 r4 |
e'4 e'4 c'4 e'4 |
g'4 b'4 g'4 c'4 |
g'4 c'4 b'4 d'4 |
e'4 c'4 g'4 d'4 |
e'4 a'4 f'4 e'4 |
b'4 f'4 d'4 f'4 |
a'4 f'4 r4 g'4 |
d'4 f'4 a'4 f'4 |
a'4 d'4 d'4 a'4 |
f'4 c'4 e'4 a'4 |
a'4 f'4 g'4 b'4 |
g'4 b'4 c'4 b'4 |
c'4 c'4 b'4 a'4 |
r4 a'4 a'4 g'4 |
b'4 a'4 g'4 c'4 |
g'4 c'4 a'4 a'4 |
f'4 c'4 c'4 e'4 |
f'4 e'4 e'4 f'4 |
}
\bar "|."
}
\new Staff {
\key c \major
\time 4/4
\clef F
g2 b2 |
e2 b2 |
c2 f2 |
e2 a2 |
c2 a2 |
e2 c2 |
g2 b2 |
g2 d2 |
b2 e2 |
c2 a2 |
b2 d2 |
b2 f2 |
e2 a2 |
e2 d2 |
f2 f2 |
b2 g2 |
c2 g2 |
e2 b2 |
b2 d2 |
c2 a2 |
\bar "|."
}
>>
\layout { }
\midi { }
}
