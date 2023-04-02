PLATFORM="Win32"
MATH="DX"

PLATFORM_SET=False
MATH_SET=False

BUILDING_OPTIONS=""

while [ "$1" != "" ]; do
	case "$1" in
		"-x86")
			if [ $PLATFORM_SET == True ]; then
				echo "You can specify only one target platform."
				exit
			fi
			PLATFORM="Win32"
            PLATFORM_SET=True
		;;
        "-x64")
			if [ $PLATFORM_SET == True ]; then
				echo "You can specify only one target platform."
				exit
			fi
			PLATFORM="x64"
            PLATFORM_SET=True
		;;
		"-SIMD_MATH_DX")
			if [ $MATH_SET == True ]; then
				echo "You can specify only one SIMD realization for math."
				exit
			fi

			MATH="DX"
			MATH_SET=True
		;;
		"-clean")
			rm -r ./Build
		;;
	esac

    shift
done

mkdir -p ./Build
cd ./Build

BUILDING_OPTIONS="$BUILDING_OPTIONS -DSIMD_MATH=$MATH"
cmake ./../ -A $PLATFORM $BUILDING_OPTIONS