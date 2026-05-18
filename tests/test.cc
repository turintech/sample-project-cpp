// Custom Catch2 main + JSON benchmark reporter.
//
// If the user does not pass --reporter, the binary defaults to writing
// console output to stdout AND a JSON benchmark report to
// artemis_results.json so benchmark runs always produce serialised output
// for downstream tools (e.g. Artemis).
//
// Catch2 (as of v3.4.0) ships no built-in JSON reporter; this file
// registers one named "artemis-json".

#include <catch2/catch_session.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/reporters/catch_reporter_streaming_base.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/benchmark/catch_estimate.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace {

std::string json_escape(std::string const& s) {
  std::string out;
  out.reserve(s.size() + 2);
  for (char c : s) {
    switch (c) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\b': out += "\\b";  break;
      case '\f': out += "\\f";  break;
      case '\n': out += "\\n";  break;
      case '\r': out += "\\r";  break;
      case '\t': out += "\\t";  break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          char buf[8];
          std::snprintf(buf, sizeof(buf), "\\u%04x", c);
          out += buf;
        } else {
          out += c;
        }
    }
  }
  return out;
}

class JsonBenchmarkReporter : public Catch::StreamingReporterBase {
 public:
  using StreamingReporterBase::StreamingReporterBase;

  static std::string getDescription() {
    return "Writes benchmark results as JSON (custom Artemis reporter)";
  }

  // Artemis's results reader applies float() to every value in the JSON,
  // so this reporter emits a flat { "<benchmark>_<metric>": <number>, ... }
  // map with no string fields and no nested structures.

  void testRunStarting(Catch::TestRunInfo const& info) override {
    StreamingReporterBase::testRunStarting(info);
    (void)info;
    m_stream << "{";
    m_first = true;
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    auto& os = m_stream;
    std::string n = json_escape(std::string(stats.info.name));
    write_kv(n + "_mean_ns",   stats.mean.point.count());
    write_kv(n + "_stddev_ns", stats.standardDeviation.point.count());
    write_kv(n + "_samples",   static_cast<double>(stats.info.samples));
    write_kv(n + "_iterations",static_cast<double>(stats.info.iterations));
    (void)os;
  }

  void benchmarkFailed(Catch::StringRef /*error*/) override {
    // Skip failed benchmarks in serialised output; Artemis only ingests numbers.
  }

  void testRunEnded(Catch::TestRunStats const& stats) override {
    (void)stats;
    m_stream << "\n}\n";
    StreamingReporterBase::testRunEnded(stats);
  }

 private:
  void write_kv(std::string const& key, double value) {
    if (!m_first) m_stream << ",";
    m_first = false;
    m_stream << "\n  \"" << key << "\": " << value;
  }

 private:
  bool m_first = true;
  std::ostream& m_stream = StreamingReporterBase::m_stream;
};

}  // namespace

CATCH_REGISTER_REPORTER("artemis-json", JsonBenchmarkReporter)

int main(int argc, char* argv[]) {
  bool user_set_reporter = false;
  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--reporter" || a == "-r" || a.rfind("--reporter=", 0) == 0) {
      user_set_reporter = true;
      break;
    }
  }

  std::vector<char*> args(argv, argv + argc);
  std::string console_arg = "console::out=-";
  std::string json_arg = "artemis-json::out=artemis_results.json";
  if (!user_set_reporter) {
    args.push_back(const_cast<char*>("--reporter"));
    args.push_back(const_cast<char*>(console_arg.c_str()));
    args.push_back(const_cast<char*>("--reporter"));
    args.push_back(const_cast<char*>(json_arg.c_str()));
  }

  return Catch::Session().run(static_cast<int>(args.size()), args.data());
}
