"""Meaningful binary round-trip and corrupt-record tests, independent of saves."""
from pathlib import Path
import importlib.util,struct,unittest

HERE=Path(__file__).resolve().parent
def module(name,file):
    spec=importlib.util.spec_from_file_location(name,HERE/file);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
pack=module('print_pack','compile-newspaper-pack.py')
container=module('asset_container','package-runtime-assets.py')

class PackTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.blob,cls.manifest=pack.compile_pack()
    def mutate(self,at,data,record=False):
        b=bytearray(self.blob);b[at:at+len(data)]=data
        if record:
            offset,length=struct.unpack_from('<II',b,28)
            struct.pack_into('<I',b,36,pack.fnv(b[offset:offset+length]))
        ni,na=struct.unpack_from('<II',b,12);struct.pack_into('<I',b,20,pack.fnv(b[24:24+ni*16+na*20]));return b
    def reject(self,b):
        with self.assertRaises((AssertionError,struct.error)):pack.inspect_pack(b)
    def test_roundtrip_all_fifty_eight_pages(self):self.assertEqual(pack.inspect_pack(self.blob),58)
    def test_deterministic_rebuild(self):self.assertEqual(pack.compile_pack()[0],self.blob)
    def test_issue_memory_cap(self):self.assertLessEqual(self.manifest['maximum_edition_bytes'],131072)
    def test_native_art_memory_cap(self):self.assertLessEqual(self.manifest['maximum_native_art_bytes'],49152)
    def test_truncated_streams(self):
        for length in (0,8,23,100,len(self.blob)-1):self.reject(self.blob[:length])
    def test_bad_magic(self):self.reject(b'BADMAGIC'+self.blob[8:])
    def test_excessive_directory(self):self.reject(self.mutate(12,struct.pack('<I',258)))
    def test_directory_checksum(self):
        b=bytearray(self.blob);b[24]^=1;self.reject(b)
    def test_record_checksum(self):
        b=bytearray(self.blob);offset=struct.unpack_from('<I',b,28)[0];b[offset+128]^=1;self.reject(b)
    def test_record_bounds(self):self.reject(self.mutate(28,struct.pack('<I',len(self.blob)-1)))
    def test_wrong_system_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];self.reject(self.mutate(offset,struct.pack('<H',255),True))
    def test_excessive_ops_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];self.reject(self.mutate(offset+124,struct.pack('<I',4097),True))
    def test_invalid_mark_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];self.reject(self.mutate(offset+128,b'\x05',True))
    def test_outside_native_frame_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];np=struct.unpack_from('<H',self.blob,offset+2)[0]
        self.reject(self.mutate(offset+384+np*12,struct.pack('<h',481),True))
    def test_unknown_opcode_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];np=struct.unpack_from('<H',self.blob,offset+2)[0]
        self.reject(self.mutate(offset+384+np*12+12,struct.pack('<H',99),True))
    def test_invalid_string_offset_inside_valid_checksum(self):
        offset=struct.unpack_from('<I',self.blob,28)[0];np=struct.unpack_from('<H',self.blob,offset+2)[0]
        self.reject(self.mutate(offset+384+np*12+16,struct.pack('<I',0xffffffff),True))
    def test_oversized_art(self):
        ni=struct.unpack_from('<I',self.blob,12)[0];self.reject(self.mutate(24+ni*16,struct.pack('<HH',256,160)))
    def test_intro_bytes_unchanged(self):
        intro=b'ENINTRO1'+b'I'*100;joined=intro+self.blob+container.footer(len(intro),self.blob)
        self.assertEqual(container.bounds(joined),(len(intro),len(self.blob)));self.assertEqual(joined[:len(intro)],intro)
    def test_bad_container_footer(self):
        intro=b'ENINTRO1'+b'I'*100;joined=bytearray(intro+self.blob+container.footer(len(intro),self.blob));joined[-1]^=1
        with self.assertRaises(AssertionError):container.bounds(joined)
    def test_legacy_intro_has_no_container(self):self.assertIsNone(container.bounds(b'ENINTRO1'+b'I'*100))

if __name__=='__main__':unittest.main(verbosity=2)
