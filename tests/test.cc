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

  void testRunStarting(Catch::TestRunInfo const& info) override {
    StreamingReporterBase::testRunStarting(info);
    auto& os = m_stream;
    os << "{\n";
    os << "  \"run\": \"" << json_escape(std::string(info.name)) << "\",\n";
    os << "  \"benchmarks\": [";
    m_first = true;
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    auto& os = m_stream;
    if (!m_first) os << ",";
    m_first = false;
    os << "\n    {";
    os << "\"name\": \"" << json_escape(std::string(stats.info.name)) << "\"";
    os << ", \"samples\": " << stats.info.samples;
    os << ", \"iterations\": " << stats.info.iterations;
    os << ", \"mean_ns\": " << stats.mean.point.count();
    os << ", \"mean_lower_ns\": " << stats.mean.lower_bound.count();
    os << ", \"mean_upper_ns\": " << stats.mean.upper_bound.count();
    os << ", \"stddev_ns\": " << stats.standardDeviation.point.count();
    os << ", \"stddev_lower_ns\": " << stats.standardDeviation.lower_bound.count();
    os << ", \"stddev_upper_ns\": " << stats.standardDeviation.upper_bound.count();
    os << "}";
  }

  void benchmarkFailed(Catch::StringRef error) override {
    auto& os = m_stream;
    if (!m_first) os << ",";
    m_first = false;
    os << "\n    {\"name\": \"" << json_escape(std::string(error)) << "\", \"failed\": true}";
  }

  void testRunEnded(Catch::TestRunStats const& stats) override {
    auto& os = m_stream;
    os << "\n  ],\n";
    os << "  \"totals\": {";
    os << "\"test_cases\": " << stats.totals.testCases.total();
    os << ", \"test_cases_passed\": " << stats.totals.testCases.passed;
    os << ", \"test_cases_failed\": " << stats.totals.testCases.failed;
    os << ", \"assertions\": " << stats.totals.assertions.total();
    os << ", \"assertions_passed\": " << stats.totals.assertions.passed;
    os << ", \"assertions_failed\": " << stats.totals.assertions.failed;
    os << "}\n";
    os << "}\n";
    StreamingReporterBase::testRunEnded(stats);
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
