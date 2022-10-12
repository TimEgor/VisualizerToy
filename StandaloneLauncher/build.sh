PLATFORM="Win32"
GRAPHIC_API="D3D12"
CONFIGURATION="Release"
MATH="DX"

PLATFORM_SET=False
MATH_SET=False
GRAPHIC_API_SET=False

BUILDING_OPTIONS=""

while [ "$1" != "" ]; do
	case "$1" in
		"-debug")
			CONFIGURATION="Debug"
		;;
		"-d3d12"|"-D3D12")
			if [ $GRAPHIC_API_SET == True ]; then
				echo "You can specify only one graphic API."
				exit
			fi
			GRAPHIC_API="D3D12"
			GRAPHIC_API_SET=True
		;;
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
		"-imgui")
			BUILDING_OPTIONS="$BUILDING_OPTIONS -DIMGUI=True"
		;;
		"-clean")
			rm -r ./Build
		;;
	esac

    shift
done

mkdir -p ./Build
cd ./Build

BUILDING_OPTIONS="$BUILDING_OPTIONS -DSIMD_MATH=$MATH -DGRAPHIC_API=$GRAPHIC_API"
cmake ./../ -A $PLATFORM -DCMAKE_BUILD_TYPE=$CONFIGURATION $BUILDING_OPTIONS