/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:28:09 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 12:28:39 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	*handle_86_64(cpu_subtype_t sub)
{
	if ((sub & ~CPU_SUBTYPE_LIB64) == CPU_SUBTYPE_X86_64_H)
		return (ft_strdup("x86_64h"));
	return (ft_strdup("x86_64"));
}

char	*cpu_name(cpu_type_t ct, cpu_subtype_t sub)
{
	if (ct == CPU_TYPE_VAX)
		return (ft_strdup("vax"));
	else if (ct == CPU_TYPE_MC680x0)
		return (ft_strdup("mc680x0"));
	else if (ct == CPU_TYPE_I386)
		return (ft_strdup("i386"));
	else if (ct == CPU_TYPE_X86_64)
		return (handle_86_64(sub));
	else if (ct == CPU_TYPE_MC98000)
		return (ft_strdup("mc98000"));
	else if (ct == CPU_TYPE_HPPA)
		return (ft_strdup("hppa"));
	else if (ct == CPU_TYPE_ARM)
		return (ft_strdup("arm"));
	else if (ct == CPU_TYPE_ARM64)
		return (ft_strdup("arm64"));
	else if (ct == CPU_TYPE_MC88000)
		return (ft_strdup("mc88000"));
	else if (ct == CPU_TYPE_SPARC)
		return (ft_strdup("sparc"));
	else if (ct == CPU_TYPE_I860)
		return (ft_strdup("i860"));
	else if (ct == CPU_TYPE_POWERPC)
		return (ft_strdup("ppc"));
	return (ft_strdup("ppc64"));
}
