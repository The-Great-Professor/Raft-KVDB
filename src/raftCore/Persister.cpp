
#include "Persister.h"
#include "util.h"
// todo:如果文件出现问题会怎么办？？
void Persister::Save(const std::string& raftstate, const std::string& snapshot) {
  std::lock_guard<std::mutex> lg(mtx);
  // 将raftstate和snapshot写入本地文件
  m_raftStateOutStream << raftstate << std::flush;
  m_snapshotOutStream << snapshot << std::flush;
}

std::string Persister::ReadSnapshot() {
  std::lock_guard<std::mutex> lg(mtx);
  if (m_snapshotOutStream.is_open()) {
    m_snapshotOutStream.close();
  }
  //    Defer ec1([this]()-> void {
  //        this->m_snapshotOutStream.open(snapshotFile);
  //    }); //这个变量后生成，会先销毁
  DEFER { m_snapshotOutStream.open(snapshotFile); };  // 这个变量后生成，会先销毁
  std::fstream ifs(snapshotFile, std::ios_base::in);
  if (!ifs.good()) {
    return "";
  }
  std::string snapshot;
  ifs >> snapshot;
  ifs.close();
  // std::cout << "snapshot: " << snapshot << std::endl;
  return snapshot;
}

void Persister::SaveRaftState(const std::string& data) {
  std::lock_guard<std::mutex> lg(mtx);

  m_raftStateOutStream << data << std::flush;
}

long long Persister::RaftStateSize() {
  std::lock_guard<std::mutex> lg(mtx);

  return m_raftStateSize;
}

std::string Persister::ReadRaftState() {
  std::lock_guard<std::mutex> lg(mtx);

  std::fstream ifs(raftStateFile, std::ios_base::in);
  if (!ifs.good()) {
    return "";
  }
  std::string snapshot;
  ifs >> snapshot;
  ifs.close();
  return snapshot;
}

Persister::Persister(int me)
    : raftStateFile("raftstatePersist" + std::to_string(me) + ".txt"),
      snapshotFile("snapshotPersist" + std::to_string(me) + ".txt"),
      m_raftStateSize(0) {
  std::fstream file1(raftStateFile, std::ios::out | std::ios::trunc);
  // std::fstream file1(raftStateFile, std::ios::out | std::ios::app);
  if (file1.is_open()) {
    file1.close();
  }
  std::fstream file2(snapshotFile, std::ios::out | std::ios::trunc);
  // std::fstream file2(snapshotFile, std::ios::out | std::ios::app);
  if (file2.is_open()) {
    file2.close();
  }

  m_raftStateOutStream.open(raftStateFile, std::ios::app);
  // m_raftStateOutStream << "start";
  m_snapshotOutStream.open(snapshotFile, std::ios::app);
  // m_snapshotOutStream << "start";
}

Persister::~Persister() {
  if (m_raftStateOutStream.is_open()) {
    m_raftStateOutStream.close();
  }
  if (m_snapshotOutStream.is_open()) {
    m_snapshotOutStream.close();
  }
}
