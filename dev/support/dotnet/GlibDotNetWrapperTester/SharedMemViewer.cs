using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using gdotnet.io;
using System.Runtime.InteropServices;
using System.Reflection;

namespace GlibDotNetWrapperTester
{
    /// <summary>
    /// 
    /// </summary>
    public partial class SharedMemViewer : Form
    {
        /// <summary>
        /// 
        /// </summary>
        public enum SuitableTypes
        {
            st_string,
            st_int32,
            st_int64,
            st_uint32,
            st_uint64,
            st_double,
            st_single
        }

        public SortedDictionary<string, SharedMemory> mDictionarySharedMem = new SortedDictionary<string, SharedMemory>();

        /// <summary>
        /// Constructor.
        /// </summary>
        public SharedMemViewer()
        {
            InitializeComponent();
            mInit();
        }

        private void TheButtonRead_Click(object sender, EventArgs e)
        {
            SharedMemory shared_memory = mGetSharedMemory();

            if (shared_memory != null)
            {
                mReadValue(shared_memory);
            }
        }


        private void TheButtonWrite_Click(object sender, EventArgs e)
        {
            SharedMemory shared_memory = mGetSharedMemory();

            if (shared_memory != null)
            {
                mWriteValue(shared_memory);
            }
        }

        private void mReadValue(SharedMemory aSharedMemory)
        {
            SuitableTypes format_type = mGetFormatType();

            object read_value = mInterpretMemory(aSharedMemory, format_type);

            if (TheCheckIsHex.Checked && mIsInteger(format_type))
            {
                TheTextSharedValue.Text = mToHex(read_value);
            }
            else
            {
                TheTextSharedValue.Text = read_value.ToString();
            }
        }

        private string mToHex(object aIntegerValue)
        {
            int type_size = Marshal.SizeOf(aIntegerValue.GetType());

            string result = "";
            Int64 start_value = System.Convert.ToInt64(aIntegerValue);

            for (int hex_digit_shift = 0; hex_digit_shift < type_size * 2; hex_digit_shift++)
            {
                Int64 mask = ((Int64)0xf << (hex_digit_shift * 4));
                Int64 masked = (start_value & mask) >> (hex_digit_shift * 4);

                char digit;

                if (masked < 10)
                {
                    digit = (char)((Int64)'0' + masked);
                }
                else
                {
                    digit = (char)((Int64)'a' + masked - 10 );
                }

                result = digit + result;
            }

            return "0x" + result;
        }

        private bool mIsInteger(SuitableTypes aFormatType)
        {
            switch (aFormatType)
            {
                case SuitableTypes.st_int32:
                case SuitableTypes.st_int64:
                case SuitableTypes.st_uint32:
                case SuitableTypes.st_uint64:
                    return true;
                default:
                    return false;
            }
        }

        private void mWriteValue(SharedMemory aSharedMemory)
        {
            SuitableTypes format_type = mGetFormatType();

            object write_value = mParseValue(TheTextSharedValue.Text, format_type);

            if (write_value != null)
            {
                mEncodeValue(write_value, format_type, aSharedMemory);
            }
            else
            {
                TheTextSharedValue.Text = "Error";
            }
        }

        private SuitableTypes mGetFormatType() { return (SuitableTypes)TheComboValueTypes.SelectedItem; }

        private object mParseValue(string aStringValue, SuitableTypes aFormatType)
        {
            if (aFormatType == SuitableTypes.st_string)
            {
                return aStringValue;
            }
            else
            {
                string type_string = aFormatType.ToString().Substring(3);

                Type type_used = Type.GetType("system." + type_string, true, true);

                MethodInfo parse_method_info = type_used.GetMethod("Parse", new Type[] { typeof(String) });

                try { return parse_method_info.Invoke(null, new object[] { aStringValue }); }
                catch (Exception) { return null; }
            }
        }

        private object mInterpretMemory(SharedMemory aSharedMemory, SuitableTypes aFormatType)
        {
            switch (aFormatType)
            {
                case SuitableTypes.st_string:
                    return Marshal.PtrToStringAnsi(aSharedMemory.theAddress);
                case SuitableTypes.st_int32:
                    return Marshal.ReadInt32(aSharedMemory.theAddress);
                case SuitableTypes.st_int64:
                    return Marshal.ReadInt64(aSharedMemory.theAddress);
                case SuitableTypes.st_uint32:
                    return (UInt32)Marshal.ReadInt32(aSharedMemory.theAddress);
                case SuitableTypes.st_uint64:
                    return (UInt64)Marshal.ReadInt64(aSharedMemory.theAddress);
                case SuitableTypes.st_double:
                    return aSharedMemory.getDouble(0);
                case SuitableTypes.st_single:
                default:
                    return aSharedMemory.getSingle(0);
            }
        }

        private void mEncodeValue(object aWriteValue, SuitableTypes aFormatType, SharedMemory aSharedMemory)
        {
            switch (aFormatType)
            {
                case SuitableTypes.st_string:
                    byte[] string_data = new byte[(aWriteValue as string).Length + 1];
                    Marshal.Copy(Marshal.StringToHGlobalAnsi(aWriteValue as string), string_data, 0, string_data.Length);
                    Marshal.Copy(string_data, 0, aSharedMemory.theAddress, string_data.Length);
                    break;
                case SuitableTypes.st_int32:
                    Marshal.WriteInt32(aSharedMemory.theAddress, (int)aWriteValue);
                    break;
                case SuitableTypes.st_int64:
                    Marshal.WriteInt64(aSharedMemory.theAddress, (Int64)aWriteValue);
                    break;
                case SuitableTypes.st_uint32:
                    Marshal.WriteInt32(aSharedMemory.theAddress, (int)aWriteValue);
                    break;
                case SuitableTypes.st_uint64:
                    Marshal.WriteInt64(aSharedMemory.theAddress, (Int64)aWriteValue);
                    break;
                case SuitableTypes.st_double:
                    aSharedMemory.writeDouble((double)aWriteValue, 0);
                    break;
                default:
                case SuitableTypes.st_single:
                    aSharedMemory.writeSingle((Single)aWriteValue, 0);
                    break;
            }
        }

        private SharedMemory mGetSharedMemory()
        {
            string shared_mem_name = TheTextSharedName.Text.Trim();

            if (mDictionarySharedMem.ContainsKey(shared_mem_name))
            {
                return mDictionarySharedMem[shared_mem_name];
            }
            else
            {
                return mDictionarySharedMem[shared_mem_name] = new SharedMemory(shared_mem_name, 1024, SharedMemFlags.create_always | SharedMemFlags.read_and_write);

            }
        }

        private void mInit()
        {
            TheComboValueTypes.Items.Clear();
            foreach (object label in Enum.GetValues(typeof(SuitableTypes)))
            {
                TheComboValueTypes.Items.Add(label);
            }

            TheComboValueTypes.SelectedIndex = 0;
        }
    }
}