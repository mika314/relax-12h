#include <fstream>
#include <iostream>
#include <log/log.hpp>
#include <midi/file.hpp>

auto main() -> int
{
  static const auto Tempo = 60.;
  auto m = midi::File{};
  m.header.format = midi::Format::one;
  m.header.division = static_cast<midi::Ticks>(4);
  m.tracks.reserve(2);
  {
    auto &track = m.tracks.emplace_back();
    track.events.emplace_back(0, 255, midi::MetaEvent{midi::MetaEventType::SequenceTrackName, "hours"});
    track.events.emplace_back(0, 255, midi::TimeSignature{4, 2, 24, 8});
    track.events.emplace_back(0, 255, static_cast<midi::SetTempo>(60 * 1'000'000 / Tempo));
    track.events.emplace_back(0, 255, midi::KeySignature{0, false});
    track.events.emplace_back(0, 0, midi::ProgramChange{33});
    for (auto h = 0; h < 12; ++h)
    {
      for (auto m5 = 0; m5 < 12; ++m5)
      {
        const auto k = (m5 * 7 + 6) % 12 - 6;
        for (auto m = 0; m < 5; ++m)
        {
          for (auto j = 0; j < 3; ++j)
          {
            for (auto i = 0; i < 4; ++i)
            {
              const auto bit = 1 << (3 - i);
              const auto n = static_cast<uint8_t>(60 + k - 12 + 7 * ((h & bit) != 0 ? 1 : 0));
              track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
              track.events.emplace_back(8, 3, midi::NoteOn{n, 0});
            }
            for (auto i = 0; i < 4; ++i)
            {
              const auto bit = 1 << (3 - i);
              const auto n = static_cast<uint8_t>(60 + k + 7 * ((h & bit) != 0 ? 1 : 0));
              track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
              track.events.emplace_back(8, 3, midi::NoteOn{n, 0});
            }
          }
          for (auto i = 0; i < 4; ++i)
          {
            const auto bit = 1 << (3 - i);
            const auto n = static_cast<uint8_t>(60 + k - 12 + 7 * ((h & bit) != 0 ? 1 : 0));
            track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
            track.events.emplace_back(8, 3, midi::NoteOn{n, 0});
          }
          for (auto i = 0; i < 2; ++i)
          {
            const auto bit = 1 << (3 - i);
            const auto n = static_cast<uint8_t>(60 + k + 7 * ((h & bit) != 0 ? 1 : 0));
            track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
            track.events.emplace_back(8, 3, midi::NoteOn{n, 0});
          }
        }
      }
    }
    track.events.emplace_back(0, 255, midi::EndOfTrack{});
  }
  {
    auto &track = m.tracks.emplace_back();
    track.events.emplace_back(0, 1, midi::ProgramChange{0});
    track.events.emplace_back(
      0, 255, midi::MetaEvent{midi::MetaEventType::SequenceTrackName, "minutes"});
    for (auto h = 0; h < 12; ++h)
    {
      for (auto m = 0; m < 60; ++m)
      {
        const auto k = (m / 5 * 7 + 6) % 12 - 6;
        for (auto j = 0; j < 3; ++j)
        {
          for (auto i = 0; i < 4; ++i)
          {
            const auto d = m / 10;
            const auto bit = 1 << (3 - i);
            const auto n = static_cast<uint8_t>(60 + k + 12 + 7 * ((d & bit) != 0 ? 1 : 0));
            track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
            track.events.emplace_back(4, 3, midi::NoteOn{n, 0});
          }
          track.events.emplace_back(4 * 4, 3, midi::NoteOn{static_cast<uint8_t>(60 + k), 0});
          for (auto i = 0; i < 4; ++i)
          {
            const auto d = m % 10;
            const auto bit = 1 << (3 - i);
            const auto n = static_cast<uint8_t>(60 + k + 2 * 12 + 7 * ((d & bit) != 0 ? 1 : 0));
            track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
            track.events.emplace_back(4, 3, midi::NoteOn{n, 0});
          }
          track.events.emplace_back(4 * 4, 3, midi::NoteOn{static_cast<uint8_t>(60 + k), 0});
        }
        for (auto i = 0; i < 4; ++i)
        {
          const auto d = m / 10;
          const auto bit = 1 << (3 - i);
          const auto n = static_cast<uint8_t>(60 + k + 12 + 7 * ((d & bit) != 0 ? 1 : 0));
          track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
          track.events.emplace_back(4, 3, midi::NoteOn{n, 0});
        }
        track.events.emplace_back(4 * 4, 3, midi::NoteOn{static_cast<uint8_t>(60 + k), 0});
        for (auto i = 0; i < 4; ++i)
        {
          const auto d = m % 10;
          const auto bit = 1 << (3 - i);
          const auto n = static_cast<uint8_t>(60 + k + 2 * 12 + 7 * ((d & bit) != 0 ? 1 : 0));
          track.events.emplace_back(0, 3, midi::NoteOn{n, 127});
          track.events.emplace_back(4, 3, midi::NoteOn{n, 0});
        }
      }
    }
    track.events.emplace_back(0, 255, midi::EndOfTrack{});
  }
  {
    auto &track = m.tracks.emplace_back();
    track.events.emplace_back(0, 2, midi::ProgramChange{0});
    track.events.emplace_back(0, 255, midi::MetaEvent{midi::MetaEventType::SequenceTrackName, "pads"});
    for (auto h = 0; h < 12; ++h)
    {
      for (auto m = 0; m < 60; ++m)
      {
        const auto k = (m / 5 * 7 + 6) % 12 - 6;
        auto n = 60 + k - 12;
        for (auto j = 0; j < 3; ++j)
        {
          track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n), 127});
          track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 127});
          track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 12), 127});
          track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 0});
          track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 127});
          track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n), 0});
          track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 0});
          track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n + 12), 0});
        }
        track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n), 127});
        track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 127});
        track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 12), 127});
        track.events.emplace_back(16, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 0});
        track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n), 0});
        track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n + 7), 0});
        track.events.emplace_back(0, 3, midi::NoteOn{static_cast<uint8_t>(n + 12), 0});
      }
    }
  }
  {
    auto &track = m.tracks.emplace_back();
    track.events.emplace_back(0, 3, midi::ProgramChange{0});
    track.events.emplace_back(0, 255, midi::MetaEvent{midi::MetaEventType::SequenceTrackName, "2th"});
    for (auto h = 0; h < 12; ++h)
    {
      for (auto m = 0; m < 60; ++m)
      {
        const auto k = (m / 5 * 7 + 6) % 12 - 6;
        for (auto j = 0; j < 120; ++j)
        {
          auto n = 60 + k + 12;
          switch (rand() % 4)
          {
          case 0: break;
          case 1: n += 7; break;
          case 2: n += 4; break;
          case 3: n += 12; break;
          }
          track.events.emplace_back(
            0,
            3,
            midi::NoteOn{static_cast<uint8_t>(n), static_cast<uint8_t>((rand() % 16 == 0) ? 127 : 0)});
          track.events.emplace_back(2, 3, midi::NoteOn{static_cast<uint8_t>(n), 0});
        }
      }
    }
  }
  {
    auto &track = m.tracks.emplace_back();
    track.events.emplace_back(0, 4, midi::ProgramChange{0});
    track.events.emplace_back(0, 255, midi::MetaEvent{midi::MetaEventType::SequenceTrackName, "8th"});
    for (auto h = 0; h < 12; ++h)
    {
      for (auto m = 0; m < 60; ++m)
      {
        const auto k = (m / 5 * 7 + 6) % 12 - 6;
        for (auto j = 0; j < 120; ++j)
        {
          auto n = 60 + k + 24;
          switch (rand() % 4)
          {
          case 0: break;
          case 1: n += 7; break;
          case 2: n += 4; break;
          case 3: n += 12; break;
          }
          track.events.emplace_back(
            0,
            3,
            midi::NoteOn{static_cast<uint8_t>(n), static_cast<uint8_t>((rand() % 4 == 0) ? 127 : 0)});
          track.events.emplace_back(2, 3, midi::NoteOn{static_cast<uint8_t>(n), 0});
        }
      }
    }
  }
  auto f = std::ofstream{"relax-12h.mid", std::ios::binary};
  m.write(f);
  LOG("format", midi::toStr(m.header.format));
  LOG("ntrks", m.header.ntrks);
  LOG("ntrks", m.tracks.size());
  LOG("division", toStr(m.header.division));
}
