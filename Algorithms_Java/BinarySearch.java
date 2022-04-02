package Algorithms_Java;

public class BinarySearch{
    // Sorted array
    
    /*static int transcript_array[] = {1645419600, 1645506000, 1645592400, 1645678800, 1645765200,
                    1645851600, 1645938000, 1646024400, 1646110800, 1646197200,
                    1646283600, 1646370000, 1646456400, 1646542800, 1646629200,
                    1646715600, 1646802000, 1646888400, 1646974800, 1647061200,
                    1647147600, 1647234000, 1647320400, 1647406800, 1647493200,
                    1647579600, 1647666000, 1647752400, 1647838800, 1647925200, 
                    1648011600, 1648098000, 1648184400, 1648270800, 1648357200,
                    1648443600, 1648530000, 1648616400, 1648702800};
        */            
    
    static int transcript_array[] = {3, 5, 7, 8,
        9, 10, 11, 13, 17,
        18, 19, 20, 21, 22,
        31, 32, 35, 37, 38,
        39, 40, 41, 42, 43,
        44, 45, 46, 47, 48, 
        49, 50, 51, 52, 53,
        54, 55, 56, 57, 59};
        
    static int sizeofTranscript = transcript_array.length;

    /**
     * @brief This function finds the Cirrus Sequence Number from the list of TSNs
     * 
     * @param CSN 
     * @return true 
     * @return false 
     */
    static boolean contains(int key) {
        int start = 0;
        int end = sizeofTranscript - 1;

        boolean status = false;
        
        while (start <= end)
        {
            int mid = start + (end - start) / 2;
            int midVal = transcript_array[mid];
    
            if (midVal < key)
            {
                
                /*
                * If mid is greater than key, all elements
                * in range [mid + 1, end] are also greater
                * so we now search in [start, mid - 1]
                */
                start = mid + 1;
            }
            else if (midVal > key)
            {
                
                /*
                 * If mid is greater than key, all elements
                 * in range [mid + 1, end] are also greater
                 * so we now search in [start, mid - 1]
                 */
                end = mid - 1;
            }
            else if (midVal == key)
            {
                
                /*
                 * Comparison added just for the sake
                 * of clarity if mid is equal to key, we
                 * have found that key exists in array
                 */
                status = true;
                break;
            }
        }
        return status;
    }

    /**
     * @brief Get the index of the CIRRUS Sequence Number from the recorded TSNs
     * 
     * @param key 
     * @return int
     */
    static int getOnsetPosition(int key) {
        int start = 0;
        int end = sizeofTranscript - 1;
        int status = -1;

        while (start <= end)
        {
            int mid = start + (end - start + 1) / 2;
            int midVal = transcript_array[mid];

            if (midVal < key)
            {
                /*
                 * If mid is less than key, all elements
                 * in range [start, mid - 1] are < key
                 * we note down the last found index, then
                 * we search in right side of mid
                 * so we now search in [mid + 1, end]
                 */
                status = mid;
                start = mid + 1;
            }
            else if (midVal > key)
            {
                /* 
                 * If mid is greater than key, all elements
                 * in range [mid + 1, end] are > key
                 * then we search in left side of mid
                 * so we now search in [start, mid - 1]
                 */
                end = mid - 1;
            }
            else if (midVal == key)
            {
                /*
                 * If mid is equal to key, all elements
                 * in range [mid + 1, end] are >= key
                 * then we search in left side of mid
                 * so we now search in [start, mid - 1]
                 */
                end = mid - 1;
            }
        }

        return status;
    }

    static boolean inRange(int key) {
        return key >= transcript_array[0] && key <= transcript_array[sizeofTranscript - 1] ? true : false;
    }

    public static void main(String[] args)
    {
        int onset = 58;
        //int onset = 1645592401;
        System.out.println("Onset: " + onset);

        if(inRange(onset)) {
            int beginRange = 0;
            int onset_position = 0;

            if(contains(onset)) {
                onset_position = getOnsetPosition(onset) + 1;
                beginRange = onset_position < 3 ? 0 : onset_position - 2;
            } else {
                onset_position = getOnsetPosition(onset);
                beginRange = onset_position == 0 || onset_position == 1 ? 0 : onset_position - 1;
            }
            
            final int endRange = ((sizeofTranscript - 1) - onset_position) < 14 ? sizeofTranscript - 1 : sizeofTranscript - ((sizeofTranscript - onset_position) - 14);

            System.out.println("Range BEGINS at index: " + beginRange + " (" + transcript_array[beginRange] + ")");
            System.out.println("Range ENDS at index: " + endRange + " (" + transcript_array[endRange] + ")");
        } else {
            System.out.println("Onset is not available in the local trstatuscripts.");
        }
    }
}