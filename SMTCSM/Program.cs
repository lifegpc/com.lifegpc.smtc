using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SMTCSM {
    class Handler {
        private NPSMLib.NowPlayingSessionManager manager;
        public bool should_closed;
        public Handler() {
            manager = new NPSMLib.NowPlayingSessionManager();
            should_closed = false;
        }
        NPSMLib.NowPlayingSession GetSession(string appId) {
            var sessions = manager.GetSessions();
            if (sessions == null || appId == null) {
                return null;
            }
            for (var i = 0; i < sessions.Length; i++) {
                var session = sessions[i];
                if (session == null) {
                    continue;
                }
                if (session.SourceAppId == null) {
                    continue;
                }
                if (session.SourceAppId != appId) {
                    continue;
                }
                return session;
            }
            return null;
        }
        NPSMLib.NowPlayingSession GetSession(int index) {
            var sessions = manager.GetSessions();
            if (sessions == null) {
                return null;
            }
            if (index >= sessions.Length) {
                return null;
            }
            return sessions[index];
        }
        NPSMLib.NowPlayingSession GetSession(ref NetworkStream stream, byte[] data, int offset) {
            if (data.Length - offset < 5) return null;
            int length = decodeLength(data, offset + 1);
            switch (data[offset]) {
                case 1:
                    return GetSession(length);
                case 2:
                    var appId = readString(ref stream, length);
                    if (appId == null) {
                        return null;
                    }
                    return GetSession(appId);
                default:
                    return manager.CurrentSession;
            }
        }
        string GetSourceAppId(NPSMLib.NowPlayingSession session) {
            if (session == null) {
                return "";
            } else if (session == null) {
                return "";
            } else {
                return session.SourceAppId;
            }
        }
        string GetSourceDeviceId(NPSMLib.NowPlayingSession session) {
            if (session == null) {
                return "";
            } else if (session.SourceDeviceId == null) {
                return "";
            } else {
                return session.SourceDeviceId;
            }
        }
        string GetRenderDeviceId(NPSMLib.NowPlayingSession session) {
            if (session == null) {
                return "";
            } else if (session.RenderDeviceId == null) {
                return "";
            } else {
                return session.RenderDeviceId;
            }
        }
        string GetTitle(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Title != null) {
                    return obj.Title;
                }
            }
            return "";
        }
        string GetArtist(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.Artist;
                }
            }
            return "";
        }
        string GetSubtitle(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.Subtitle;
                }
            }
            return "";
        }
        string GetAlbumArtist(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.AlbumArtist;
                }
            }
            return "";
        }
        string GetAlbumTitle(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.AlbumTitle;
                }
            }
            return "";
        }
        string GetMediaClassPrimaryId(NPSMLib.NowPlayingSession session) {
            if (session == null) return "";
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.MediaClassPrimaryID;
                }
            }
            return "";
        }
        string[] GetGenres(NPSMLib.NowPlayingSession session) {
            if (session == null) return new string[0];
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.Genres;
                }
            }
            return new string[0];
        }
        uint GetAlbumTrackCount(NPSMLib.NowPlayingSession session) {
            if (session == null) return 0;
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.AlbumTrackCount;
                }
            }
            return 0;
        }
        uint GetTrackNumber(NPSMLib.NowPlayingSession session) {
            if (session == null) return 0;
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                var obj = data.GetMediaObjectInfo();
                if (obj.Artist != null) {
                    return obj.TrackNumber;
                }
            }
            return 0;
        }
        Stream GetThubnailStream(NPSMLib.NowPlayingSession session) {
            if (session == null) return null;
            var data = session.ActivateMediaPlaybackDataSource();
            if (data != null) {
                return data.GetThumbnailStream();
            }
            return null;
        }
        byte[] GetThubnailStreamHash(NPSMLib.NowPlayingSession session) {
            var stream = GetThubnailStream(session);
            if (stream != null) {
                byte[] hash = calHash(ref stream);
                stream.Dispose();
                return hash;
            }
            return new byte[0];
        }
        string GetThubnailFilename(NPSMLib.NowPlayingSession session) {
            var stream = GetThubnailStream(session);
            if (stream != null) {
                var path = Path.GetTempFileName();
                var s = File.Create(path);
                stream.CopyTo(s);
                stream.Close();
                s.Close();
                stream.Dispose();
                s.Dispose();
                return path;
            }
            return "";
        }
        string readString(ref NetworkStream stream, int length) {
            byte[] buffer = new byte[length];
            int readed = readData(ref stream, buffer, 0, length);
            return decodeString(buffer, 0, readed);
        }
        int readData(ref NetworkStream stream, byte[] buffer, int offset, int length) {
            int max_readed = buffer.Length - offset;
            if (length < max_readed) max_readed = length;
            int readed = 0;
            while (readed < max_readed) {
                readed += stream.ReadAsync(buffer, offset + readed, max_readed - readed).Result;
            }
            return readed;
        }
        int decodeLength(byte[] buffer, int offset) {
            if (!BitConverter.IsLittleEndian) {
                byte[] temp = new byte[4];
                Array.Copy(buffer, offset, temp, 0, 4);
                Array.Reverse(temp);
                return BitConverter.ToInt32(temp, 0);
            } else {
                return BitConverter.ToInt32(buffer, offset);
            }
        }
        void encodeLength(byte[] buffer, int offset, int length) {
            byte[] data = BitConverter.GetBytes(length);
            if (!BitConverter.IsLittleEndian) {
                Array.Reverse(data);
            }
            Array.Copy(data, 0, buffer, offset, 4);
        }
        void encodeUint(byte[] buffer, int offset, uint data) {
            byte[] d = BitConverter.GetBytes(data);
            if (!BitConverter.IsLittleEndian) {
                Array.Reverse(d);
            }
            Array.Copy(d, 0, buffer, offset, 4);
        }
        byte[] encodeUint(uint data) {
            byte[] d = new byte[4];
            encodeUint(d, 0, data);
            return d;
        }
        string decodeString(byte[] str, int offset, int length) {
            Encoding utf8 = Encoding.UTF8;
            return utf8.GetString(str, offset, length);
        }
        byte[] encodeString(string str) {
            Encoding utf8 = Encoding.UTF8;
            byte[] buffer = utf8.GetBytes(str);
            int length = buffer.Length;
            byte[] result = new byte[length + 4];
            encodeLength(result, 0, length);
            Array.Copy(buffer, 0, result, 4, length);
            return result;
        }
        byte[] encodeStrings(string[] strs) {
            Encoding utf8 = Encoding.UTF8;
            int array_len = strs.Length;
            int length = 0;
            List<byte[]> enstrs = new List<byte[]>();
            for (int i = 0; i < array_len; i++) {
                byte[] tmp = utf8.GetBytes(strs[i]);
                enstrs.Add(tmp);
                length += tmp.Length;
            }
            byte[] result = new byte[4 + array_len * 4 + length];
            encodeLength(result, 0, array_len);
            int offset = 4;
            for (int i = 0; i < array_len; i++) {
                encodeLength(result, offset, enstrs[i].Length);
                offset += 4;
                Array.Copy(enstrs[i], 0, result, offset, enstrs[i].Length);
                offset += enstrs[i].Length;
            }
            return result;
        }
        byte[] calHash(ref Stream stream) {
            SHA512Managed sha512 = new SHA512Managed();
            sha512.ComputeHash(stream);
            byte[] result = sha512.Hash;
            sha512.Dispose();
            return result;
        }
        public bool Handle(ref TcpClient client) {
            var stream = client.GetStream();
            try {
                bool looped = true;
                while (looped) {
                    byte[] buffer = new byte[6];
                    stream.ReadAsync(buffer, 0, buffer.Length).Wait();
                    byte action = buffer[0];
                    byte[] data = null;
                    switch (action) {
                        case 0:
                            data = encodeString(GetSourceAppId(GetSession(ref stream, buffer, 1)));
                            break;
                        case 1:
                            data = encodeString(GetSourceDeviceId(GetSession(ref stream, buffer, 1)));
                            break;
                        case 2:
                            data = encodeString(GetRenderDeviceId(GetSession(ref stream, buffer, 1)));
                            break;
                        case 3:
                            data = encodeString(GetTitle(GetSession(ref stream, buffer, 1)));
                            break;
                        case 4:
                            data = encodeString(GetArtist(GetSession(ref stream, buffer, 1)));
                            break;
                        case 5:
                            data = encodeString(GetSubtitle(GetSession(ref stream, buffer, 1)));
                            break;
                        case 6:
                            data = encodeString(GetAlbumArtist(GetSession(ref stream, buffer, 1)));
                            break;
                        case 7:
                            data = encodeString(GetAlbumTitle(GetSession(ref stream, buffer, 1)));
                            break;
                        case 8:
                            data = encodeString(GetMediaClassPrimaryId(GetSession(ref stream, buffer, 1)));
                            break;
                        case 9:
                            data = encodeStrings(GetGenres(GetSession(ref stream, buffer, 1)));
                            break;
                        case 10:
                            data = encodeUint(GetAlbumTrackCount(GetSession(ref stream, buffer, 1)));
                            break;
                        case 11:
                            data = encodeUint(GetTrackNumber(GetSession(ref stream, buffer, 1)));
                            break;
                        case 12:
                            data = GetThubnailStreamHash(GetSession(ref stream, buffer, 1));
                            break;
                        case 13:
                            data = encodeString(GetThubnailFilename(GetSession(ref stream, buffer, 1)));
                            break;
                        case 0xfe:
                            looped = false;
                            should_closed = true;
                            break;
                        case 0xff:
                            looped = false;
                            break;
                    }
                    if (data == null) {
                        data = new byte[1];
                    }
                    stream.WriteAsync(data, 0, data.Length).Wait();
                    System.GC.Collect();
                }
            } catch (Exception e) {
                Console.WriteLine(e);
            } finally {
                stream.Close();
            }
            stream.Dispose();
            return true;
        }
    }
    class SMTCSM {
        static void ThreadProc(ref Handler handler, ref TcpClient client) {
            handler.Handle(ref client);
            client.Dispose();
        }
        static async Task Main(string[] args) {
            var handler = new Handler();
            TcpListener server = null;
            ThreadPool.SetMaxThreads(4, 8);
            try {
                int port = 23412;
                IPAddress localAddr = IPAddress.Parse("127.0.0.1");
                server = new TcpListener(localAddr, port);
                server.Start();
                while (true) {
                    var client = await server.AcceptTcpClientAsync();
                    ThreadPool.QueueUserWorkItem(new WaitCallback((_) => ThreadProc(ref handler, ref client)));
                    if (handler.should_closed) {
                        break;
                    }
                }
            } catch (Exception e) {
                Console.WriteLine(e);
            } finally {
                if (server != null) {
                    server.Stop();
                }
            }
        }
    }
}
