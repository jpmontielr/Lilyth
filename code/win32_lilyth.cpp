// TODO(jp): Dotted notes!!!
// TODO(jp): A good way to input relative mode, for closer notes in large octave ranges.
// TODO(jp): Ties instead of notes in odd beat places.
// TODO(jp): Triplets.
// TODO(jp): Big one: Simpler chords for their inversions.

#include <stdio.h>   // fopen, fprintf, etc.
#include <stdlib.h>  // srand, rand
#include <windows.h> // rdtsc (as srand seed), BitScanForward
#include <stdint.h>  // int8_t, int16_t, etc.

#define internal static 
#define local_persist static 
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Assert(Expression)  if(!(Expression)) { *(int *)0 = 0; }

enum interval
{
    Interval_Unison,
    Interval_Second,
    Interval_Third,
    Interval_Fourth,
    Interval_Fifth,
    Interval_Sixth,
    Interval_Seventh,
    Interval_Octave,
    Interval_Ninth,
    Interval_Tenth,
};

struct music_output
{
    char **ScaleNotes;
    int ScaleNoteCount;
    char **OctaveSuffixes;
    int OctaveSuffixCount;
    interval *ChordIntervals;
    int ChordIntervalCount;
    
    uint32 TimeSignatureNumerator;
    uint32 TimeSignatureDenominator;

    int BaseOctave;
    int OctaveRange;
    
    bool32 MakeRandomDurations;
    uint32 MinRandomDuration;
    
    uint32 AverageNotesPerRest;
    uint32 AverageNotesPerChord;

    uint32 FixedDuration;
};

internal void
InsertNote(FILE *FileName, music_output *MusicOutput, uint32 Duration)
{
    bool32 IsNote = ((rand() % (MusicOutput->AverageNotesPerRest + 1)) != 0);
    if(IsNote)
    {
        int NoteIndex =  (rand() % MusicOutput->ScaleNoteCount);
        int NoteOctaveIndex = MusicOutput->BaseOctave + (rand() % MusicOutput->OctaveRange);
    
        Assert(NoteIndex < MusicOutput->ScaleNoteCount);
        Assert(NoteOctaveIndex < MusicOutput->OctaveSuffixCount);

        char *Note = MusicOutput->ScaleNotes[NoteIndex];
        char *NoteOctave = MusicOutput->OctaveSuffixes[NoteOctaveIndex];

        bool32 IsChord = ((rand() % (MusicOutput->AverageNotesPerChord + 1)) == 0);
        if(IsChord && MusicOutput->ChordIntervals && MusicOutput->ChordIntervalCount)
        {
            char *NoteFormatString = "%s%s ";
            fprintf(FileName, "<");
            fprintf(stdout, "<");
            fprintf(FileName, NoteFormatString, Note, NoteOctave);
            fprintf(stdout, NoteFormatString, Note, NoteOctave);
                        
            for(int IntervalIndex = 0;
                IntervalIndex < MusicOutput->ChordIntervalCount;
                ++IntervalIndex)
            {
                int BaseNoteIndex = NoteIndex;
                int BaseNoteOctaveIndex = NoteOctaveIndex;
                        
                int UnwrappedIntervalNoteIndex = (BaseNoteIndex +
                                                  MusicOutput->ChordIntervals[IntervalIndex]);
                int IntervalNoteIndex = (UnwrappedIntervalNoteIndex % MusicOutput->ScaleNoteCount);
                int IntervalNoteOctaveIndex = (BaseNoteOctaveIndex +
                                               (UnwrappedIntervalNoteIndex / MusicOutput->ScaleNoteCount));
    
                Assert(IntervalNoteIndex <= MusicOutput->ScaleNoteCount);
                Assert(IntervalNoteOctaveIndex < MusicOutput->OctaveSuffixCount);
        
                char *IntervalNote = MusicOutput->ScaleNotes[IntervalNoteIndex];
                char *IntervalNoteOctave = MusicOutput->OctaveSuffixes[IntervalNoteOctaveIndex];

                fprintf(FileName, NoteFormatString, IntervalNote, IntervalNoteOctave);
                fprintf(stdout, NoteFormatString, IntervalNote, IntervalNoteOctave);
            }
            fprintf(FileName, ">%d ", Duration);
            fprintf(stdout, ">%d ", Duration);
        }
        else
        {
            char *NoteFormatString = "%s%s%d ";
            fprintf(FileName, NoteFormatString, Note, NoteOctave, Duration);
            fprintf(stdout, NoteFormatString, Note, NoteOctave, Duration);
        }
    }
    else
    {        
        char *FormatString = "r%d ";
        fprintf(FileName, FormatString, Duration);
        fprintf(stdout, FormatString, Duration);
    }
}

internal void
InsertNotes(FILE *FileName, music_output *MusicOutput, uint32 MeasureCount)
{
    for(uint32 MeasureIndex = 0;
        MeasureIndex < MeasureCount;
        ++MeasureIndex)
    {
        real32 FractionOfAWholeNotePerMeasure = ((real32)MusicOutput->TimeSignatureNumerator/
                                                   (real32)MusicOutput->TimeSignatureDenominator);
        
        if(MusicOutput->MakeRandomDurations)
        {
            real32 RemainingFractionOfAWholeNote = FractionOfAWholeNotePerMeasure;
            while(RemainingFractionOfAWholeNote > 0.0f)
            {
                int MaxDurationShift = 0;
                if(RemainingFractionOfAWholeNote >= 1.0f)
                {
                    MaxDurationShift = 0;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/2.0f)
                {
                    MaxDurationShift = 1;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/4.0f)
                {
                    MaxDurationShift = 2;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/8.0f)
                {
                    MaxDurationShift = 3;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/16.0f)
                {
                    MaxDurationShift = 4;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/32.0f)
                {
                    MaxDurationShift = 5;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/64.0f)
                {
                    MaxDurationShift = 6;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/128.0f)
                {
                    MaxDurationShift = 7;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/256.0f)
                {
                    MaxDurationShift = 8;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/512.0f)
                {
                    MaxDurationShift = 9;
                }
                else if(RemainingFractionOfAWholeNote >= 1.0f/1024.0f)
                {
                    MaxDurationShift = 10;
                }
                else
                {
                    // NOTE(jp): This is the (RemainingFractionOfAWholeNote < 1.0f/1024.0f) case, which should
                    // only mean it is 0.0f, which should be catched by the enclosing while(RemainingFractionOfAWholeNote > 0.0f)
                    Assert(!"Whaaat\n");
                }

                // TODO(jp): Make a non-Windows version of this.
                DWORD BitScanForwardResult;
                BitScanForward(&BitScanForwardResult, MusicOutput->MinRandomDuration);
                int MinDurationShift = (int)BitScanForwardResult;

                // TODO(jp): Repair this hack, the assertion can fire under some circumstances.
                int RandomDuration = MinDurationShift;
                if(MinDurationShift > MaxDurationShift)
                {
                    int RandomShift = (rand() % ((MinDurationShift - MaxDurationShift) + 1));
                    RandomDuration = ((1 << MaxDurationShift) << RandomShift);
                }
                else if(MinDurationShift == MaxDurationShift)
                {
                    RandomDuration = (1 << MinDurationShift);                    
                }
                else
                {
                    Assert(!"The duration granularity failed this time");
                }

                real32 DurationFraction = 1.0f / RandomDuration;
                InsertNote(FileName, MusicOutput, RandomDuration);
                RemainingFractionOfAWholeNote -= DurationFraction;
            }
            fprintf(FileName, "|\n");
            fprintf(stdout, "|\n");            
        }
        else
        {
            uint32 FixedDurationsPerMeasure = (uint32)(FractionOfAWholeNotePerMeasure/
                                                       (1.0f/(real32)MusicOutput->FixedDuration));

            for(uint32 BeatIndex = 0;
                BeatIndex < FixedDurationsPerMeasure;
                ++BeatIndex)
            {
                InsertNote(FileName, MusicOutput, MusicOutput->FixedDuration);
            }
            fprintf(FileName, "|\n");
            fprintf(stdout, "|\n");
        }
    }
}

int main(int ArgCount, char **Args)
{
    srand((unsigned int)__rdtsc());

    FILE *LilyPondFile = fopen("lilyth.ly", "w");

    fprintf(LilyPondFile, "\\version \"2.24.4\"\n");
    
    fprintf(LilyPondFile, "\\header {\n");
    fprintf(LilyPondFile, "title = \"Sight Reading Practice\"\n");
    fprintf(LilyPondFile, "composer = \"Juan P. Montiel\"\n");
    fprintf(LilyPondFile, "}\n");
    
    fprintf(LilyPondFile, "\\score \n");
    fprintf(LilyPondFile, "{\n");

    fprintf(LilyPondFile, "\\new PianoStaff \\with { instrumentName = \"Piano\" } << \n");

    // NOTE(jp): You can create any scale with the lilypond "es" and "is" note suffixes.
    // You can create longer scales, or put in just the notes you want to limit note output to a range.
    char *CMajor[] = {"c", "d", "e", "f", "g", "a", "b"};
    char *AMinor[] = {"c", "d", "e", "f", "g", "gis", "a", "b"};
    char *OctaveSuffixes[] = { ",,,", ",,", ",", "", "'", "''", "'''", "''''", "'''''"};

    // NOTE(jp): These should all be intervals from the base note. It is kind of confusing
    // that a musical interval of a "second" has a distance of 1 from the base note,
    // a "fourth" has a distance of 3, an "octave" has a distance of 7, etc. Of course then,
    // a unison has a distance of 0.  Use the interval enum values to be less confused.
    interval ChordIntervals[] = {Interval_Third};
    
    music_output MusicOutput = {};
    MusicOutput.ScaleNotes = CMajor;
    MusicOutput.ScaleNoteCount = ArrayCount(CMajor);
    MusicOutput.OctaveSuffixes = OctaveSuffixes;
    MusicOutput.OctaveSuffixCount = ArrayCount(OctaveSuffixes);
    MusicOutput.ChordIntervals = 0;//ChordIntervals;
    MusicOutput.ChordIntervalCount = ArrayCount(ChordIntervals);
    MusicOutput.TimeSignatureNumerator = 4;
    MusicOutput.TimeSignatureDenominator = 4;
    MusicOutput.BaseOctave = 4;
    // NOTE(jp): This is how many octaves to randomly choose from above the BaseOctave, must be at least 1.
    MusicOutput.OctaveRange = 1;
    // NOTE(jp): If MakeRandomDurations is false, the FixedDuration is used, but if MakeRandomDurations
    // is true, FixedDuration is ignored and random note durations (checked!) are outputted, using
    // MinDuration as the smallest note duration.
    MusicOutput.MakeRandomDurations = false;
    MusicOutput.MinRandomDuration = 4;
    MusicOutput.AverageNotesPerRest = 32;
    MusicOutput.AverageNotesPerChord = 64;
    MusicOutput.FixedDuration = 4;

    uint32 MeasureCount = 20;

    // NOTE(jp): Right hand
    fprintf(stdout, "\nRIGHT HAND:\n\n");
    
    fprintf(LilyPondFile, "\\new Staff {\n");
    fprintf(LilyPondFile, "\\tempo \"Variable\" 4 = 60-120\n");
    fprintf(LilyPondFile, "\\transpose c c {\n");    
    fprintf(LilyPondFile, "\\key c \\major\n");    
    fprintf(LilyPondFile, "\\time %d/%d\n", MusicOutput.TimeSignatureNumerator, MusicOutput.TimeSignatureDenominator);
    fprintf(LilyPondFile, "\\clef G\n");

    InsertNotes(LilyPondFile, &MusicOutput, MeasureCount);
    fprintf(LilyPondFile, "}\n"); // \transpose

    fprintf(LilyPondFile, "\\bar \"|.\"\n");
    fprintf(LilyPondFile, "}\n"); // right hand \new Staff

#if 1
    // NOTE(jp): Left hand
    fprintf(stdout, "\n\nLEFT HAND:\n\n");

    fprintf(LilyPondFile, "\\new Staff {\n");
    fprintf(LilyPondFile, "\\key c \\major\n");    
    fprintf(LilyPondFile, "\\time %d/%d\n", MusicOutput.TimeSignatureNumerator, MusicOutput.TimeSignatureDenominator);
    fprintf(LilyPondFile, "\\clef F\n");
//    fprintf(LilyPondFile, "\\transpose c c {\n");    
    
    MusicOutput.BaseOctave = 3;
    MusicOutput.ChordIntervalCount = 0;
    MusicOutput.MakeRandomDurations = false;
    MusicOutput.MinRandomDuration = 4;
    MusicOutput.AverageNotesPerRest = 32;
    MusicOutput.FixedDuration = 2;

    InsertNotes(LilyPondFile, &MusicOutput, MeasureCount);
    
//    fprintf(LilyPondFile, "}\n"); // \transpose test
    fprintf(LilyPondFile, "\\bar \"|.\"\n");
    fprintf(LilyPondFile, "}\n"); // left hand \new Staff
#endif
     
    /// NOTE(jp): Ending.
    fprintf(LilyPondFile, ">>\n");
    
    fprintf(LilyPondFile, "\\layout { }\n");
    fprintf(LilyPondFile, "\\midi { }\n");
    
    fprintf(LilyPondFile, "}\n"); // \score
    fclose(LilyPondFile);

    return(0);
}
