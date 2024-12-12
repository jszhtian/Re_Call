using System;
using System.Collections.Generic;
using System.Linq;

namespace VToolBase.Core {
	public static class LinqHelpers {
		public static bool IsOneOf<T>(this T value, params T[] options) =>
			options.Contains(value);

		public static TValue TryGet<TKey, TValue>(this Dictionary<TKey, TValue> dict, TKey key, TValue fallback = default) =>
			dict.ContainsKey(key) ? dict[key] : fallback;

		public static TValue Fetch<TKey, TValue>(this Dictionary<TKey, TValue> dict, TKey key, Func<TValue> producer) =>
			dict.ContainsKey(key) ? dict[key] : producer();

		public static string JoinString<T>(this IEnumerable<T> values, string separator) =>
			string.Join(separator, values);
	}
}
