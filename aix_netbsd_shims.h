
#ifndef _AIX_NETBSD_SHIMS_H_
#define _AIX_NETBSD_SHIMS_H_

#ifdef _AIX

    #include <sys/vmalloc.h>
    #include <sys/types.h>

    /* we also tack on other useful consts */
    //#define KM_SLEEP (MA_OK2SLEEP | MA_LONGTERM)
    #define KM_SLEEP (MA_OK2SLEEP)



    #define __P(x) x

    typedef caddr_t vm_offset_t;

    typedef size_t vm_size_t;

    /* there's no struct device on this platform. however, we could make one 
    to store basic bookkeeping information that we need */

    struct device {
        short dv_unit;
        char * dv_xname;
    };        

    typedef struct device device_t;

    typedef int (*cfmatch_t) __P((struct device *, void *, void *));

    struct cfdriver {
        void	**cd_devs;		/* devices found */
        char	*cd_name;		/* device name */
        cfmatch_t cd_match;		/* returns a match level */
        void	(*cd_attach) __P((struct device *, struct device *, void *));
        //enum	devclass cd_class;	/* device classification */
        int cd_class;
        size_t	cd_devsize;		/* size of dev data (for malloc) */
        int	cd_indirect;		/* indirectly configure subdevices */
        int	cd_ndevs;		/* size of cd_devs array */
    };

    #define DV_DULL 1

    #define b_data b_un.b_addr

    caddr_t kmem_alloc(u_int flags, size_t size);

    /* sc->sc_mem = malloc(16384, M_DEVBUF, M_NOWAIT); */
    //sc->sc_mem = kmemalloc(16384, MA_PAGE | MA_LONGTERM);

    // kernel_map in netbsd is a literal vm memory map; we're mapping it to flags
    #define kernel_map (MA_PAGE | MA_LONGTERM)

    #include <sys/user.h>
    #include <sys/vmparam.h>

    void biodone (struct buf *bp);


#endif


#endif
